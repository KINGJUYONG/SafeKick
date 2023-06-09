"""rest_server URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.urls import include, re_path, path
from django.contrib import admin
from rest_framework import routers
from rest_framework_swagger.views import get_swagger_view
from timer import views

from django.contrib import admin
from django.conf import settings
from django.conf.urls.static import static



import timer.api

app_name='timer'

router = routers.DefaultRouter()
router.register('timers', timer.api.TimerViewSet)


urlpatterns = [
    re_path(r'^api/v1/', include((router.urls, 'timer'), namespace='api')),
    path('submit', views.ReflectAPI().as_view()),
    path('verify', views.helmetAPI().as_view()),

] 

