from django.contrib import admin

from courses.models import DM, Assignment, Chat, Course, FileSubmission, Forum, Message #, Instructor, Student

# Register your models here.

admin.site.register(Course)
admin.site.register(Assignment)
admin.site.register(FileSubmission)
admin.site.register(Message)
admin.site.register(Forum)
admin.site.register(Chat)
admin.site.register(DM)


#admin.site.register(Student)
#admin.site.register(Instructor)