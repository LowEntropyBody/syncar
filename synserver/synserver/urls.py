"""synserver URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin
from deal import views as deal_views  # new

urlpatterns = [
	url(r'^admin/', admin.site.urls),
	url(r'^uploadinfor/', deal_views.uploadinfor,name='uploadinfor'),
	url(r'^start/', deal_views.start,name='start'),
	url(r'^end/', deal_views.end,name='end'),
	url(r'^controlstart/', deal_views.controlstart,name='controlstart'),
	url(r'^getstatus/', deal_views.getstatus,name='getstatus'),
]
