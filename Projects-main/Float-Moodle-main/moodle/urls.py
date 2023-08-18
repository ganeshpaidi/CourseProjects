"""moodle URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.2/topics/http/urls/
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
from django.contrib import admin
from django.urls import path, include
from . import views
from moodle import settings

from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('admin/', admin.site.urls),
    path('',views.dashboard,name = 'dashboard'),
    path('signup/',views.sign_up,name = 'signup'),
    path('accounts/',include('accounts.urls')),
    path('courses/',include('courses.urls')),
    path('profile_change/',views.profile_change,name = 'profile_change'),
    path('change_pass/',views.change_pass,name = 'change_pass'),
    path('chat/',views.chat,name = 'chat'),
    path('chat/<int:id1>/<int:id2>/',views.DirMsg,name = 'DM'),
    path('chat/CLI',views.bot,name = 'cli'),
]

if settings.DEBUG:
    urlpatterns += static(settings.STATIC_URL, document_root = settings.STATIC_ROOT)
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
