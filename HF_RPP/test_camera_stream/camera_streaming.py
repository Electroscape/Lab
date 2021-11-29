from flask import Flask, render_template
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app)


@app.route('/', methods=['POST', 'GET'])
def index():
    return render_template('index.html')


def main():
    ssl_context = ('test.cert', 'test.key')
    socketio.run(app, host='0.0.0.0', debug=False, ssl_context=ssl_context)


if __name__ == '__main__':
    main()


