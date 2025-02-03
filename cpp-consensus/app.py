import os
from flask import Flask

app = Flask(__name__)

# Must match the path in main.cpp
log_path = "blocks.log"

@app.route('/')
def dashboard():
    content = ""
    if os.path.exists(log_path):
        with open(log_path, "r") as f:
            content = f.read()

    html = f"""
    <html>
    <head>
        <title>Regulator Dashboard</title>
        <meta http-equiv="refresh" content="5">
    </head>
    <body>
        <h1>Regulator Dashboard</h1>
        <pre>{content}</pre>
    </body>
    </html>
    """
    return html

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)
