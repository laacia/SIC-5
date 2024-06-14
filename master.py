import json
from flask import Flask, request, jsonify

app = Flask(__name__)

TEMP_DATA = []
HUM_DATA = []

@app.route("/")
def root_route():
    return "Hello world!"

@app.route("/temp")
def get_temp():
    return jsonify(TEMP_DATA)

@app.route("/hum")
def get_hum():
    return jsonify(HUM_DATA)

@app.route("/submit-temp", methods=["POST"])
def post_temp():
    pesan = request.data.decode("utf8")
    TEMP_DATA.append(float(pesan))
    print(pesan)
    return f"Received temperature {pesan}"

@app.route("/submit-hum", methods=["POST"])
def post_hum():
    pesan = request.data.decode("utf8")
    HUM_DATA.append(float(pesan))
    print(pesan)
    return f"Received humidity {pesan}"

@app.route("/submit", methods=["POST"])
def post_data():
    pesan = request.data.decode("utf8")
    pesan = json.loads(pesan)
    TEMP_DATA.append(float(pesan["temp"]))
    HUM_DATA.append(float(pesan["hum"]))
    print(pesan)
    return f"Received data {pesan}"

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0')
