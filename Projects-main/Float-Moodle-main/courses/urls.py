from . import views
from django.urls import path , include


urlpatterns = [
    path('', views.course_info, name = 'CourseHome'),
    path('create/',views.create, name = 'CourseCreate'),
    path('join/',views.join,name = 'CourseJoin'),
    path('<int:course_id>/',views.course,name = 'coursepage'),
    path('<int:course_id>/create',views.assign_create,name ='assign_create'),
    path('<int:course_id>/assign/<int:assign_id>',views.assign,name ='assign_page'),
    path('<int:course_id>/assign/<int:assign_id>/submissions',views.submissions,name ='submissions'),
    path('<int:course_id>/assign/<int:assign_id>/feedback/<int:sub_id>',views.feedback,name ='feedback'),
    path('<int:course_id>/participants',views.participants,name ='participants'),
    path('<int:course_id>/<int:forum_id>/',views.forum,name ='forum'),
    path('disable/<int:course_id>/<int:forum_id>/',views.disable,name = 'disable'),
    path('remove/<int:course_id>/<int:user_id>',views.remove,name = 'remove'),  
    path('<int:course_id>/<int:user_id>/tasettings/',views.Settings,name = 'TA_settings'), 
    path('<int:course_id>/assign/<int:assign_id>/submissions/filefeedback',views.file_feedback,name ='file_feedback'),


    path('<int:course_id>/assign/<int:assign_id>/submissions/autograde',views.autograde,name ='autograde'),
    path('<int:course_id>/grades',views.grades,name ='grades'),


    path('<int:course_id>/alert',views.alert,name = 'alert'),
        
]