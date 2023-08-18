from django.contrib.auth.models import User
from courses.models import Course,Chat,Assignment,FileSubmission,DM
from django.shortcuts import redirect
import datetime

def reply(question,user_id):
    Q = question
    while Q[0] == ' ' :
        Q = Q[1:]
    while Q[-1] == ' ' or Q[-1] == '\n' or Q[-1] == '\r':
        Q = Q[:-1] 
    Q = Q.lower()

    u = User.objects.get(pk = user_id)
    I = u.ins_courses.all()
    S = u.stud_courses.all()
    T = u.ta_courses.all()
    ans = ''

    if Q in ['hi','hiii','hii','hello','hey']:
        ans = 'Hello 👋 ' + u.username
    elif Q == 'my-name':
        ans = u.username
    elif Q == 'my-email':
        ans = u.email
    elif Q == 'my-first-name':
        ans = u.first_name
    elif Q == 'my-last-name':
        ans = u.last_name
    elif Q == 'my-profile':
        ans = 'Name:  ' + u.username + '\n' + 'Email: ' + u.email + '\n' + 'First Name: ' + u.first_name + '\n' + 'Last Name: ' + u.last_name
    elif Q == 'who-r-u':
        ans = 'I am CLI-BOT 🤖, U can ask me queries and I will try my best to answer them'
    

    elif Q in  [ 'list-courses','ls','courses','my-courses']:

        ans = ''
        ans += 'INSTRUCTOR: [ '
        for i in I:
            ans += (i.name + ' , ') 
        ans += ' ]\n'
        ans += 'STUDENT: [ '
        for i in S:
            ans += (i.name + ' , ') 
        ans += ' ]\n'
        ans += 'TA: [ '
        for i in T:
            ans += (i.name + ' , ') 
        ans += ' ]\n'

    elif Q in [ 'list-stud-courses','ls -s','courses -s']:
        ans = ''
        ans += 'STUDENT: [ '
        for i in S:
            ans += (i.name + ' , ') 
        ans += ' ]\n'
    
    elif Q in [ 'list-ins-courses','ls -i','courses -i']:
        ans = ''
        ans += 'INSTRUCTOR: [ '
        for i in I:
            ans += (i.name + ' , ') 
        ans += ' ]\n'

    elif Q in [ 'list-ta-courses','ls -t','courses -t']:
        ans = ''
        ans += 'TA: [ '
        for i in T:
            ans += (i.name + ' , ') 
        ans += ' ]\n'

    elif Q in [ 'users-list', 'ul','list-users','users-all']:
        ans = ''
        i = 1
        for u in User.objects.all():
            if(u.username != 'Bot'):
                ans += str(i) + ' : ' + u.username + '\n'
                i += 1
    
    #send-msg-<username>-msg.

    elif Q == 'ls -a -all':
            
            for c in list(I) + list(S) + list(T):
                ans = 'Assignemnts in Course: ' + c.name + '\n'
                AA = c.assignment_set.all()
                for n in AA:
                    ans += n.name + '\n'
                ans += '\n'


    elif Q.startswith('send-msg'):
        L = question.split('-',3)
        un = L[2]
        msg = L[3]

        E = User.objects.all().filter(username = L[2])
        if E.count() == 0:
            return 'User Not Found'
        else:
            usr2 = E[0]
            usr1 = u

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
        d = DM.objects.create(user=u,message = L[3],chat = C)
        d.save()
        return 'Message Sent Successfully'	

    elif Q.startswith('ls -a -'):
        cs = question[7:]
        X = Course.objects.all().filter(name = cs)
        if X.count() == 0:
            return 'Course: <' + cs + '> NOT FOUND'
        else:
            for c in X:
                if u in [c.instructor] or u in c.TAs.all() or u in c.students.all():
                    ans = 'Assignemnts in Course: ' + cs + '\n'
                    AA = c.assignment_set.all()
                    for n in AA:
                        ans += n.name + '\n'

    elif Q.startswith('ls -ad -'):
        cs = question[8:]
        X = Course.objects.all().filter(name = cs)
        if X.count() == 0:
            return 'Course: <' + cs + '> NOT FOUND'
        else:
            for c in X:
                if u in [c.instructor] or u in c.TAs.all() or u in c.students.all():
                    ans = 'TO-DO Assignemnts in Course: ' + cs + '\n'
                    AA = c.assignment_set.all()
                    now = datetime.datetime.now()
                    for n in AA:
                        if now < n.deadline:
                            f = n.filesubmission_set.all()
                            submi = False
                            for s in f:
                                if s.user == u:
                                    submi = True
                            if not submi:
                                ans += 'Assign:  ' + n.name +' , Deadline:' + str(n.deadline)+ '\n'

    elif Q.startswith('ls -ag -'):
        cs = question[8:]
        X = Course.objects.all().filter(name = cs)
        if X.count() == 0:
            return 'Course: <' + cs + '> NOT FOUND'
        else:
            for c in X:
                if  u in c.students.all():
                    ans = 'Assignemnts,Grades in Course: ' + cs + '\n'
                    AA = c.assignment_set.all()
                    for n in AA:
                        F = n.filesubmission_set.all()
                        y = True
                        for f in F:
                            if f.user == u:
                                ans += 'Assign:  '  + n.name + ',  Grade:' +str(f.grade) +  '\n'
                                y = False  
                        if y:
                            ans += 'Assign:  '  + n.name + ',  Grade:' + ' NOT SUBMITTED'  + '\n'
                                                          

    
    else:
        return 'Sorry! , I cant answer that😕'

    return ans

