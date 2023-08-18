from django.db import models
from django.db.models.signals import post_save
# Create your models here.

from django.contrib.auth.models import  User
from django.db.models.deletion import CASCADE
from django.db.models.fields import DateTimeField
from django.db.models.fields.related import ForeignKey
from django.db.models.signals import post_save

import random

class Course(models.Model):

    instructor = models.ForeignKey(User,on_delete=models.CASCADE,related_name='ins_courses') #assuming 1 instructor/course.
    name = models.CharField(max_length=128,default = '')
    students = models.ManyToManyField(User,related_name='stud_courses')
    TAs = models.ManyToManyField(User,related_name='ta_courses')
    joincode = models.CharField(max_length=10,default = '')
    TAjoincode = models.CharField(max_length=10,default = '')

    def __str__(self):
        return self.name

class Assignment(models.Model):
    name = models.CharField(max_length=128,default='')
    link = models.URLField(default='')
    file_name = models.CharField(max_length=100,default='problem_statement')
    file = models.FileField(upload_to='assignments',blank=True)
    statement = models.CharField(max_length=300,default='')
    course = models.ForeignKey(Course,on_delete=models.CASCADE)
    maxmarks = models.IntegerField(default=100)
    deadline = models.DateTimeField(blank=False) #need to set while creating assign.
    weightage = models.IntegerField(default=0)
    feedfile = models.FileField(upload_to='feedback', blank = True)

    def __str__(self):
        return self.name

class FileSubmission(models.Model):
    user = models.ForeignKey(User,on_delete=models.CASCADE)
    file = models.FileField(upload_to='submissions',blank=True)
    file_name = models.CharField(max_length=100,default='submission')
    assignment = models.ForeignKey(Assignment,on_delete=models.CASCADE)
    feedback = models.CharField(max_length=500,default='Your Feedback Appears Here')
    corrected = models.CharField(max_length=10,default='NO')
    grade = models.IntegerField(default=-1)
    sub_time = models.DateTimeField(auto_now=True) #last 'MODIFIED' time. // automatically updates itself.


    def __str__(self):
        return self.user.username +'_'  + self.file_name + '_' + self.assignment.name



class Forum(models.Model):
    name = models.CharField(max_length=20,default='FORUM')
    course = models.ForeignKey(Course,on_delete=models.CASCADE)
    enabled = models.CharField(max_length=4,default='YES')#***********************************************************

#distingush types of mesages and give color.

class Message(models.Model):
    user = models.ForeignKey(User,on_delete=models.CASCADE)
    message = models.CharField(max_length=500,default='**')
    time = models.DateTimeField(auto_now_add = True)
    forum = models.ForeignKey(Forum,on_delete=models.CASCADE)



class Chat(models.Model):
    users = models.ManyToManyField(User,related_name = 'chats')



class DM(models.Model):
    user = models.ForeignKey(User,on_delete=models.CASCADE)
    message = models.CharField(max_length=500,default='**')
    time = models.DateTimeField(auto_now_add = True)
    chat = models.ForeignKey(Chat,on_delete=models.CASCADE)


class TA_Permission(models.Model):
    user = models.ForeignKey(User,on_delete=CASCADE)
    course = models.ForeignKey(Course,on_delete=CASCADE)
    assign_create = models.CharField(max_length=4,default='NO')
    forum_create = models.CharField(max_length=4,default='NO')
    add_students = models.CharField(max_length=4,default='NO')


# post_save.connect(create_forum,sender = Course)

# def create_forum(sender,**kwargs):

'''USE SIGNALS'''
#add app to the list. and migrations admin.py etc...

