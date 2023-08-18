from django.shortcuts import render , redirect , get_object_or_404

from django.http import HttpResponse, response
from django.contrib.auth.models import User
from django.contrib.auth import update_session_auth_hash
from django.contrib.auth.forms import UserCreationForm , PasswordChangeForm
from moodle.forms import SignupForm,ProfileChangeForm,Messageform
import requests

from courses.models import Chat, DM
from . import clibot

def dashboard(request):

	if not request.user.is_authenticated:
		return redirect('login')

	return render(request,'accounts/dashboard.html')



#view corresponding to the signup page
def sign_up(request):
	if request.method == 'POST':
		#form = UserCreationForm(request.POST)
		form = SignupForm(request.POST)
		if form.is_valid():
			form.save()
			return redirect('/accounts/login/')
		else:
			#form = UserCreationForm()
			form = SignupForm()
			args = { 'form': form , 'success' : False }
			return render(request,'accounts/sign_up.html',args)
	else:
		#form = UserCreationForm()
		form = SignupForm()
		args = {'form': form , 'success' : True }
		return render(request,'accounts/sign_up.html',args)


def profile_change(request):

	if not request.user.is_authenticated:
		return redirect('login')

	if request.method == 'POST':
		form = ProfileChangeForm(request.POST,instance = request.user)
		if form.is_valid():
			form.save()
			return redirect('dashboard')

	else:
		form = ProfileChangeForm(instance = request.user)
	args = {'form': form }
	return render(request,'accounts/profile_change.html',args)


def change_pass(request):
	
	if not request.user.is_authenticated:
		return redirect('login')

	if request.method == 'POST':
		form = PasswordChangeForm(data = request.POST,user = request.user)
		if form.is_valid():
			form.save()
			update_session_auth_hash(request, form.user) 
			return redirect('dashboard')
		else:
			form = PasswordChangeForm(user = request.user)
			args = {'form': form , 'success': False}
			return render(request,'accounts/password_change.html',args)

	else:
		form = PasswordChangeForm(user = request.user)
		args = {'form': form , 'success': True}
		return render(request,'accounts/password_change.html',args)



def chat(request):

	if not request.user.is_authenticated:
		return redirect('login')

	u = User.objects.all()

	args = {
		'users': u,
		'l':[1,2,3,4,5,6,7,8,9],
		'bot': User.objects.get(username = 'Bot'),
	}

	return render(request,'accounts/chat.html',args)

def DirMsg(request,id1,id2):

	usr1 = request.user
	usr2 = User.objects.get(pk = id2)

	S1 = usr1.chats.all()
	S2 = usr2.chats.all()
	S = S1 & S2

	if len(S) == 0:
		C = Chat.objects.create()
		C.users.add(usr1)
		C.users.add(usr2)
		C.save()
	
	S1 = usr1.chats.all()
	S2 = usr2.chats.all()
	S = S1 & S2
	C = S[0]	

	if request.method == 'POST':
		form = Messageform(request.POST)
		if form.is_valid():
			M = DM.objects.create(user = request.user,chat = C)
			M.message = form.cleaned_data['message']

			M.save()
			
			return redirect('DM',id1,id2)
	else:
		form = Messageform()

	args = {
		'form': form,
		'msgs': C.dm_set.all(),
		'user2': usr2,
	}
	return render(request,'accounts/DM.html',args)


#_________________________________________*****************
def bot(request):
	usr1 = request.user
	usr2 = User.objects.get(username = 'Bot')

	S1 = usr1.chats.all()
	S2 = usr2.chats.all()
	S = S1 & S2

	if len(S) == 0:
		C = Chat.objects.create()
		C.users.add(usr1)
		C.users.add(usr2)
		C.save()
	
	S1 = usr1.chats.all()
	S2 = usr2.chats.all()
	S = S1 & S2
	C = S[0]	

	if request.method == 'POST':
		form = Messageform(request.POST)
		if form.is_valid():
			M = DM.objects.create(user = request.user,chat = C)
			M.message = form.cleaned_data['message']
			M.save()
			

			clean = M.message.replace(" ",'')
			clean = clean.replace('\n','')
			clean = clean.replace('\r','')
			if M.message.lower() == 'clear' or M.message == 'clean':
				for msg in C.dm_set.all():
					msg.delete()
			else:
				R = DM.objects.create(user = usr2,chat = C)
				reply = clibot.reply(M.message,usr1.pk)
				R.message = reply
				R.save()
			
			return redirect('cli')
	else:
		form = Messageform()

	args = {
		'form': form,
		'msgs': C.dm_set.all(),
		'user2': usr2,
	}
	return render(request,'accounts/bot.html',args)