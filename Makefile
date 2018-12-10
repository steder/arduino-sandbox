server_cert:
	openssl req -x509 -newkey rsa:4096 -nodes -out cert.pem -keyout key.pem -days 365

venv:
	mkdir -p ~/.virtualenvs
	python3 -m venv ~/.virtualenvs/plant
	source ~/.virtualenvs/plant/bin/activate
	pip3 install -U -r requirements.txt

runserver:
	gunicorn --log-level debug --certfile cert.pem --keyfile key.pem -b 0.0.0.0:8443 plant:app
