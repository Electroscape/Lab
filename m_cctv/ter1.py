import os
import sys

import requests

file_dir = os.path.dirname(__file__)
sys.path.append(file_dir)

from flask import request, Flask, render_template, send_from_directory, url_for, redirect
from flask_flatpages import FlatPages
from flask_socketio import SocketIO
from fns import js_r

from pages import app_pages, get_login_user, get_version
import socketio
import logging
from datetime import datetime as dt

now = dt.now()
log_name = now.strftime("logs/T1 %m_%d_%Y  %H_%M_%S.log")
logging.basicConfig(filename=log_name, level=logging.ERROR,
                    format=f'%(asctime)s %(levelname)s %(name)s %(threadName)s : %(message)s')

app = Flask(__name__)
app.config['SECRET_KEY'] = 'EscapeTerminal#'

ip_conf = js_r("ip_config.json", from_static=False, add_buttons=False)

terminal_name = "TR1"  # which config file to load

config = {
    "title": "Server Terminal",
    "id": "server",
    "lang": "en",
    "version": "idk"
}


@app.route('/', methods=['GET', 'POST'])
def entry_point():  # begin of the code
    return render_template("TR1/p_cctv.html", g_config=config, cams=ip_conf["cams"])


@app.route('/get_globals', methods=['GET', 'POST'])
def get_globals():
    g_config = js_r(f"json/{terminal_name}_config_{g_lang}.json", auth=login_user)
    g_config["lang"] = g_lang

    if show_personal_r == "show" and login_user == "rachel":
        tmp = {
            "title": "Personal R.",
            "details": "personal files of rachel",
            "image": "static/imgs/home/information.png",
            "link": "personal_rachel",
            "auth": False
        }
        if g_lang.lower() == "de":
            tmp["title"] = "PRIVAT R."

        g_config["btns"].append(configure_btn(tmp))
        logging.debug(g_config)

    return g_config


app.register_blueprint(app_pages)
flatpages = FlatPages(app)
app.config.from_object(__name__)

if __name__ == "__main__":
    # sio is socketio.Client(), it has no run method
    app.run(debug=True, host='0.0.0.0', port=5000)
