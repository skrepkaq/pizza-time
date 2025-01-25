import json
import time

import cherrypy
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait

from mock_data import get_mock_data

# тестовый режим с возможностью заказа питсы с гравием(!)
MOCK = False

API_URL = 'https://api.papajohns.ru'
SELENIUM_URL = 'http://pizza-proxy-selenium:4444/wd/hub'


class PapaJohnsProxy(object):
    @cherrypy.expose
    def default(self, *args, **kwargs):
        path = cherrypy.request.path_info
        method = cherrypy.request.method
        headers: dict = cherrypy.request.headers
        query_string = cherrypy.request.query_string
        data = cherrypy.request.body.read() if cherrypy.request.body.length else None

        if MOCK:
            time.sleep(1)
            return json.dumps(get_mock_data(path))

        url = f'{API_URL}{path}'

        if query_string:
            url += f'?{query_string}'

        if content_type := headers.get('Content-Type', ''):
            content_type = f"xhr.setRequestHeader('Content-Type', '{content_type}');"

        json_data = json.dumps(json.loads(data.decode())) if data else ''

        script = f'''
        var xhr = new XMLHttpRequest();
        xhr.open('{method}', '{url}', true);
        {content_type}
        xhr.onreadystatechange = function() {{
            if (xhr.readyState == 4) {{
                window.result = xhr.responseText;
                window.status = xhr.status;
            }}
        }};
        xhr.send('{json_data}');
        '''

        driver = webdriver.Remote(SELENIUM_URL, options=webdriver.ChromeOptions())
        driver.get(API_URL)
        driver.execute_script(script)

        result = WebDriverWait(driver, 10).until(
            lambda driver: driver.execute_script('return window.result;')
        )
        cherrypy.response.status = driver.execute_script('return window.status;')
        driver.quit()

        return result


if __name__ == '__main__':
    cherrypy.config.update({'server.socket_host': '0.0.0.0', 'server.socket_port': 5009})
    cherrypy.quickstart(PapaJohnsProxy())
