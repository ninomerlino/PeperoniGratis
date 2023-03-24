# importazione delle librerie necessarie
import http.server
import socketserver
import os

# definizione di una costante col numero di porta
PORT = 80
# definizione di una costante col nome della cartella root
ROOT_FOLDER = 'root'

# settaggio della cartella root
web_dir = os.path.join(os.path.dirname(__file__), ROOT_FOLDER)
os.chdir(web_dir)

if __name__ == '__main__':
    try:
        # istanziamento del server
        Handler = http.server.SimpleHTTPRequestHandler
        httpd = socketserver.TCPServer(("", PORT), Handler)
        print("serving at port", PORT) # log di successo
        # servizio attivo fino all'interruzione manuale del programma
        httpd.serve_forever()
    except KeyboardInterrupt:
        print('^C received, shutting down the server')
        httpd.server_close()