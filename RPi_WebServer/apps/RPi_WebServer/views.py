import django

def home():
	template = loader.get_template('RPi_WebServer/index.html')
	return HttpResponse(render(template))
