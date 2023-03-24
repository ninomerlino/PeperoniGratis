from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import datetime

# Define the port number
PORT = 8000

# Define the filename to store the data
FILENAME = "root/data.json"

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        """
        Handle GET requests by reading the data from the file and returning it
        """
        if self.path == '/':
            try:
                with open(FILENAME, 'r') as f:
                    data = f.read()
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    self.wfile.write(data.encode())
            except IOError:
                self.send_error(404, 'File Not Found: %s' % self.path)

    def do_POST(self):
        """
        Handle POST requests by extracting the data, formatting it in JSON object and saving it in the file
        """
        if self.path == '/':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            try:
                data = json.loads(post_data.decode())
                save_data_to_file(data)
                self.send_response(200)
                self.send_header('Content-type', 'text/html')
                self.end_headers()
                self.wfile.write(b'Data received successfully')
            except ValueError:
                self.send_error(400, 'Bad Request: Invalid JSON')

def save_data_to_file(data):
    """
    Save the received data to the file in JSON format
    """
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
    batteryChargeLevel = data.get('batteryChargeLevel')
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    soilMoisture = data.get('soilMoisture')
    # Create a dictionary with the data
    sensor_data = {
        "timestamp": timestamp,
        "batteryChargeLevel": batteryChargeLevel,
        "temperature": temperature,
        "humidity": humidity,
        "soilMoisture": soilMoisture
    }
    # Append the data to the file
    with open(FILENAME, 'a') as f:
        f.write(json.dumps(sensor_data) + '\n')

if __name__ == '__main__':
    try:
        server = HTTPServer(('', PORT), RequestHandler)
        print('Started HTTP server on port', PORT)
        server.serve_forever()
    except KeyboardInterrupt:
        print('^C received, shutting down the server')
        server.socket.close()
