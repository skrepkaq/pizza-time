Import('env')
import os
import shutil

if not os.path.isfile("include/config.h"):
    shutil.copy("config.example", "include/config.h")
