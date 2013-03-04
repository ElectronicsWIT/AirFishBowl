# -*- coding: utf-8 -*-

from django.conf import settings
from django.conf.urls.defaults import *
from benchmark_app.views import *

urlpatterns = patterns('',
	url(r'^$', home, name='home'),
)
