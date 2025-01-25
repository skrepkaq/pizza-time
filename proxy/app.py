import json
import time

import cherrypy
import requests

from mock_data import get_mock_data

# тестовый режим с возможностью заказа питсы с гравием(!)
MOCK = False

API_URL = 'https://api.papajohns.ru'


class PapaJohnsProxy(object):
    @cherrypy.expose
    def default(self, *args, **kwargs):
        path = cherrypy.request.path_info
        method = cherrypy.request.method
        headers = dict(cherrypy.request.headers)
        query_string = cherrypy.request.query_string
        data = cherrypy.request.body.read() if cherrypy.request.body.length else None

        if MOCK:
            time.sleep(1)
            return json.dumps(get_mock_data(path))

        url = f'{API_URL}{path}'

        if query_string:
            url += f'?{query_string}'

        headers_to_forward = {
            k: v for k, v in headers.items()
            if k.lower() not in ['host', 'content-length']
        }

        try:
            response = requests.request(
                method=method,
                url=url,
                headers=headers_to_forward,
                data=data,
                timeout=10
            )

            cherrypy.response.status = response.status_code
            return response.text

        except requests.exceptions.RequestException as e:
            cherrypy.response.status = 500
            return json.dumps({"error": str(e)})


if __name__ == '__main__':
    cherrypy.config.update({'server.socket_host': '0.0.0.0', 'server.socket_port': 5009})
    cherrypy.quickstart(PapaJohnsProxy())
