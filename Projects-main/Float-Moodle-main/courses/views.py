import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

from io import BytesIO
import base64

import numpy as  np

from django.http.response import HttpResponse
from django.shortcuts import redirect, render
from django.db.models.signals import post_save
from django.contrib.auth.models import User

import courses
from .forms import AssignCreationForm, AssignmentSubmitForm, CourseCreationForm , CourseJoinForm, FeedbackForm,ForumCreateForm, TaPermissionForm, UserRemoveForm
from .models import Assignment, Course, FileSubmission, Forum, Message, TA_Permission #, Student , Instructor
import random
import datetime

from moodle.forms import Messageform
# Create your views here.



def generate_code(): #function to generate random course code.
    s = ""
    for i in range(0,6):
        c = random.randint(65,90)
        s += chr(c)
    return s

'''--------------------------------------------------------------'''

#courses_home
#added-Student an instructor ToDO list
#added TA courses as well ... 24/11.
#added percentages--to render corrctly....26/11.
def course_info(request):

    if not request.user.is_authenticated:
        return redirect('login')

    u = User.objects.get(pk = request.user.pk )

    Itodo_List = [] #list of TO-DO assignments for 'I'nstructors ['S' ==> students, 'I' ==> Instructors ]
    Stodo_List = [] #list of TO-DO of students
    TAtodo_List = [] #for TA's


    PercI_List = [] #list of percentage of course completed.
    PercS_List = []
    PercT_List = []

    now = datetime.datetime.now()
    for course in u.ins_courses.all():
        graded_count = 0
        assign_count = 0
        for assign in course.assignment_set.all():
            if now > assign.deadline:
                graded=True
                for sub in assign.filesubmission_set.all():
                    if sub.corrected == 'NO':
                        graded = False
                        break
                if not graded:
                    Itodo_List.append(assign)
                else:
                    graded_count += 1
            assign_count += 1
        if assign_count == 0:
            PercI_List.append(100)
        else:
            PercI_List.append(int((graded_count*100)/assign_count))

    for course in u.stud_courses.all():
        assign_count = 0
        sub_count = 0
        for assign in course.assignment_set.all():
            if now < assign.deadline:
                submitted=False
                for sub in assign.filesubmission_set.all():
                    if sub.user.username == request.user.username:
                        submitted = True
                        break
                if not submitted:
                    Stodo_List.append(assign)
                else:
                    sub_count += 1
            else:
                submitted=False
                for sub in assign.filesubmission_set.all():
                    if sub.user.username == request.user.username:
                        submitted = True
                        break
                if submitted:
                    sub_count += 1
            assign_count += 1
        if assign_count == 0:
            PercS_List.append(100)
        else:
            PercS_List.append(int((sub_count*100)/assign_count))

    for course in u.ta_courses.all():
        graded_count = 0
        assign_count = 0
        for assign in course.assignment_set.all():
            if now > assign.deadline:
                graded=True
                for sub in assign.filesubmission_set.all():
                    if sub.corrected == 'NO':
                        graded = False
                        break
                if not graded:
                    TAtodo_List.append(assign)
                else:
                    graded_count += 1
            assign_count += 1
        if assign_count == 0:
            PercT_List.append(100)
        else:
            PercT_List.append(int((graded_count*100)/assign_count))

    for i in range(len(PercS_List)):
        PercS_List[i] = str(PercS_List[i]) + '%'

    for i in range(len(PercI_List)):
        PercI_List[i] = str(PercI_List[i]) + '%'

    for i in range(len(PercT_List)):
        PercT_List[i] = str(PercT_List[i]) + '%'


    args = {
        'scourses': u.stud_courses.all(),
        'icourses': u.ins_courses.all(),
        'Tcourses': u.ta_courses.all(),
        'Itodo': Itodo_List,
        'Stodo': Stodo_List,
        'Ttodo': TAtodo_List,
        'Perc_S': PercS_List,
        'Perc_I': PercI_List,
        'Perc_T': PercT_List,
    }
        
    return render(request,'courses/home.html',args)

#created TA join code as well
def create(request):

    if not request.user.is_authenticated:
        return redirect('login')

    if request.method == 'POST':
        form = CourseCreationForm(request.POST)
        # check whether it's valid:
        if form.is_valid():
            course_name = form.cleaned_data['name']
            code = generate_code()
            TAcode = generate_code()

            e = Course.objects.all().filter(joincode__in = [code, TAcode] ).count()
            n = Course.objects.all().filter(TAjoincode__in = [code, TAcode] ).count()
            while e != 0 or n != 0:
                code = generate_code()
                TAcode = generate_code()
                e = Course.objects.all().filter(joincode__in = [code, TAcode] ).count()
                n = Course.objects.all().filter(TAjoincode__in = [code, TAcode] ).count()

            e = Course.objects.all().filter(name = course_name).count()
            if( e != 0): #Name already exists.
                args = {'form': form, 'wrong': True}
                return render(request, 'courses/create.html', args)

            c = Course.objects.create(instructor = request.user)
            c.name = course_name
            c.joincode = code
            c.TAjoincode = TAcode
            c.save()

            return redirect('CourseHome')

    else:
        form = CourseCreationForm()

    return render(request, 'courses/create.html', {'form': form, 'wrong': False})
'---------------------------------------------------------------------------'

#added TA join also.
#TA  PERMISSIONS ALSO.
def join(request):

    if not request.user.is_authenticated:
        return redirect('login')

    if request.method == 'POST':
        form = CourseJoinForm(request.POST)
        # check whether it's valid:
        if form.is_valid():

            code = form.cleaned_data['code']
            e = Course.objects.all().filter(joincode = code).count()
            n = Course.objects.all().filter(TAjoincode = code).count()

            if( e == 0 and n == 0):  #incorrect course code.
                args = {'form': form, 'wrong': True}
                return render(request, 'courses/join.html', args)
            
            if e != 0:
                c = Course.objects.get(joincode = code)
                c.students.add(request.user)
                c.save()
            elif n != 0:
                c = Course.objects.get(TAjoincode = code)
                c.TAs.add(request.user)
                c.save()

                P = TA_Permission.objects.create(user = request.user,course = c)
                P.save()             

            return redirect('CourseHome')

    else:
        form = CourseJoinForm()

    return render(request, 'courses/join.html', {'form': form ,'wrong': False })

'--------------------------------------------------------------'

#course_page
#Addded TA check! .. 24/11
#added now time, to color the assignments.
#added forum craetion form.
#TA PERMISSIONS/.
def course(request,course_id):
    
    
    if not request.user.is_authenticated:
        return redirect('login')
    
    c = Course.objects.get(pk = course_id)

    if request.method == 'POST':
        form = ForumCreateForm(request.POST)
        if form.is_valid():
            F = Forum.objects.create(course = c)
            F.name = form.cleaned_data['name']
            F.save()
            return redirect('coursepage',course_id)
    else:
        form = ForumCreateForm()


    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in (Snames + Tnames) and uname != c.instructor.username:
        return redirect('CourseHome')

    TA_CA = 'NO'
    TA_CF = 'NO'
    if uname in Tnames:
        Q = TA_Permission.objects.all().filter(user = request.user,course = c)
        P = Q[0]
        if P.assign_create == 'YES':
            TA_CA = 'YES'
        if P.forum_create == 'YES':
            TA_CF = 'YES'

    args = {
        'Instructor': c.instructor,
        'Students': c.students.all(),
        'TAs': c.TAs.all(),
        'course': c,
        'assignments': c.assignment_set.all(),
        'nowtime': datetime.datetime.now(),
        'form': form,
        'forums': c.forum_set.all(),
        'TA_CA': TA_CA,
        'TA_CF': TA_CF,
    }
    return render(request,'courses/coursepage.html',args)


#added the assign_deadline[ in form too] while saving form into an object. 22/11____23/11[bug]
#_______FINISHED_______
def assign_create(request,course_id):
    
    if not request.user.is_authenticated:
        return redirect('login')
        
    c = Course.objects.get(pk = course_id)
    if c.instructor.username != request.user.username:
        return redirect('CourseHome')  

    if request.method == 'POST':
        form  = AssignCreationForm(request.POST,request.FILES)
        if form.is_valid():
            assign_name = form.cleaned_data['name']
            ref = form.cleaned_data['weblink']
            msg = form.cleaned_data['statement']
            dedline = form.cleaned_data['deadline']
            weight = form.cleaned_data['weightage']
            #f_name = form.cleaned_data['file_name']
            a_file = request.FILES.get('file',False)

            a = Assignment.objects.create(name = assign_name,course = c,deadline = dedline,weightage = weight)
            a.statement = msg
            a.link = ref
            a.maxmarks = form.cleaned_data['marks']

            if a_file:   #checking if file exists or not.
                a.file  = a_file
                a.file_name = a.file.name.split("/")[-1]
            a.save()

            return redirect('coursepage',course_id = c.pk)
    else:
        form = AssignCreationForm()

    args = {
        'Instructor': c.instructor,
        'Students': c.students.all(),
        'course': c,
        'form': form,
        'assignments': c.assignment_set.all()
    }
    return render(request,'courses/assign_create.html',args)


#added the deadline to render 22/11.
def assign(request,course_id,assign_id):
    a = Assignment.objects.get(pk = assign_id)
    c = Course.objects.get(pk = course_id )

    if not request.user.is_authenticated:
        return redirect('login')

    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in (Snames + Tnames) and uname != c.instructor.username:
        return redirect('CourseHome')


    if request.method == "POST":
        form = AssignmentSubmitForm(request.POST,request.FILES)
        if form.is_valid():
            s_file = request.FILES.get('file',False)

            if FileSubmission.objects.filter(user = request.user,assignment = a).exists():#resubmission.
                s = FileSubmission.objects.get(user = request.user,assignment = a)
                s.corrected = 'NO'
                s.grade = -1
            else:
                s = FileSubmission.objects.create(user = request.user, assignment = a)
            if s_file:
                s.file = s_file
                s.file_name = s.file.name.split("/")[-1]
            s.save()
                
        return redirect('assign_page',course_id = c.pk,assign_id = a.pk)
    else:
        form = AssignmentSubmitForm()

    
    subms = list(a.filesubmission_set.all())
    submitted = False
    for sub in subms:
        if request.user.username == sub.user.username:
            submitted = True
            break
    if submitted:
        mysubmission = FileSubmission.objects.get(user = request.user,assignment = a)
    else:
        mysubmission = {'feedback': 'You Did Not Submit'}

    instruct = True
    if c.instructor.username != request.user.username:
        instruct = False

    IsTA = False
    if uname in Tnames:
        IsTA = True

    deadline = a.deadline
        
    args = {
        'form': form,
        'assign': a,
        'course': c,
        'submissions': list(a.filesubmission_set.all()),
        'submitted': submitted,
        'mysub': mysubmission,
        'instruct': instruct,
        'IsTA': IsTA,
        'deadline': deadline,
        'nowtime': datetime.datetime.now(),
        'forums': c.forum_set.all(),
        'students': c.students.all(),
    }
    return render(request,'courses/assign_page.html',args)


def submissions(request,course_id,assign_id):
    a = Assignment.objects.get(pk = assign_id)
    c = Course.objects.get(pk = course_id )

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    
    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in Tnames and uname != c.instructor.username:
        return redirect('CourseHome')

    if request.method == 'POST':
        form = AssignmentSubmitForm(request.POST,request.FILES)
        if form.is_valid():
            s_file = request.FILES.get('file',False)
            if s_file:
                a.feedfile = s_file
                a.save()
                give_feedback(c,a)
                return redirect('submissions',course_id,assign_id)
    else:
        form = AssignmentSubmitForm()

    grading_done = True
    for sub in a.filesubmission_set.all():
        if sub.corrected == 'NO':
            grading_done = False
            break

    marks = assign_marks_array(c,a)

    min_marks = np.amin(marks) if  marks.size != 0 else 0
    max_marks = np.amax(marks) if  marks.size != 0 else 0
    cap = a.maxmarks

    M_mean = marks.mean()
    M_sd = marks.std()
    M_var = M_sd*M_sd

    if True:
        ax = plt.figure().gca()
        ax.yaxis.set_major_locator(MaxNLocator(integer=True))
        plt.hist(marks,bins = int(cap/10),edgecolor = 'k',linewidth = 2,color='blue',alpha = 0.68,label='Marks')
        plt.xlabel('Whole Marks Range',fontsize = 15)
        plt.ylabel('Frequency',fontsize = 15)
        plt.title('Histogram of obtained Marks, Mean',fontsize = 18)
        plt.xlim([-10,cap+10])
        plt.grid(axis='y', alpha=0.5)
        plt.axvline(marks.mean(), color= '#01153E', linestyle='dashed', linewidth=2.5,label = 'mean')
        ax.legend(loc='lower center', bbox_to_anchor=(0.5, 0.1),
            ncol=3, fancybox=True, shadow=True)

        

        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        histm = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

    if True:
        ax = plt.figure().gca()
        ax.yaxis.set_major_locator(MaxNLocator(integer=True))
        plt.hist(marks,bins = int(cap/10),edgecolor = 'k',linewidth = 2,color='r',alpha = 0.7,label='Marks')
        plt.xlabel('Obtained Marks Range',fontsize = 15)
        plt.ylabel('Frequency',fontsize = 15)
        plt.title('Histogram of obtained Marks, Mean',fontsize = 18)
        plt.xlim([min_marks-5,max_marks+5])
        plt.grid(axis='y', alpha=0.5)
        plt.axvline(marks.mean(), color= 'k', linestyle='dashed', linewidth=2.5,label = 'mean')
        ax.legend(loc='lower center', bbox_to_anchor=(0.5, 0.1),
            ncol=3, fancybox=True, shadow=True)

        
        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        histv = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

                
    args = {
        'assign': a,
        'course': c,
        'submissions': list(a.filesubmission_set.all()),
        'form': form,
        'nowtime': datetime.datetime.now(),
        'GRADED': grading_done,
        'histv':histv,
        'histm':histm,
        'mean': round(M_mean,2),
        'sd': round(M_sd,2),
        'var': round(M_var,2),
    }
    return render(request,'courses/submissions.html',args)

def feedback(request,course_id,assign_id,sub_id):
    a = Assignment.objects.get(pk = assign_id)
    c = Course.objects.get(pk = course_id )
    s = FileSubmission.objects.get(pk = sub_id)

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username

    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in Tnames and uname != c.instructor.username:
        return redirect('CourseHome')


    if request.method == "POST":
        form = FeedbackForm(request.POST)
        if form.is_valid():
            print(form.cleaned_data['feedback'])
            s.feedback = form.cleaned_data['feedback']
            s.corrected = 'YES'
            s.grade = form.cleaned_data['grade']
            s.save()

            return redirect('submissions',course_id,assign_id)
    else:
        if s.corrected == "YES":
            form = FeedbackForm({'feedback': s.feedback})
        else:
            form = FeedbackForm()

    args = {
        'assign': a,
        'course': c,
        'submissions': list(a.filesubmission_set.all()),
        'sub': s,
        'form': form
    }
    return render(request,'courses/feedback.html',args)


    #CHANGE PASSWORD ETC..

##FINISHED
def forum(request,course_id,forum_id):

    if not request.user.is_authenticated:
        return redirect('login')
        
    C = Course.objects.get(pk = course_id)
    F = Forum.objects.get(pk = forum_id)

    Snames = [ s.username for s in C.students.all() ]
    Tnames = [ s.username for s in C.TAs.all() ]
    uname = request.user.username
    if uname not in (Snames + Tnames) and uname != C.instructor.username:
        return redirect('CourseHome')

    if request.method == 'POST':
        form = Messageform(request.POST)
        if form.is_valid():
            M = Message.objects.create(user = request.user,forum = F)
            M.message = form.cleaned_data['message']
            M.save()
            
            return redirect('forum',course_id,forum_id)
    else:
        form = Messageform()

    IsTA = 'NO'
    for u in C.TAs.all():
        if request.user.username == u.username:
            IsTA = 'YES'
            break


    args = {
        'course': C,
        'Forum': F,
        'form': form,
        'msgs': F.message_set.all(),
        'Ins': C.instructor,
        'IsTA': IsTA,
    }
    return render(request,'courses/forum.html',args)

#helper for forum.
def disable(request,course_id,forum_id):

    C = Course.objects.get(pk = course_id)
    F = Forum.objects.get(pk = forum_id)

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    if uname != C.instructor.username:
        return redirect('CourseHome')

    if F.enabled == 'YES':
        F.enabled = 'NO'
    else:
        F.enabled = 'YES'
    F.save()

    return redirect('forum',course_id,forum_id)

def participants(request,course_id):

    c = Course.objects.get(pk = course_id)

    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in (Snames + Tnames) and uname != c.instructor.username:
        return redirect('CourseHome')


    if request.method == 'POST':
        form = UserRemoveForm(request.POST)
        if form.is_valid():
            uname = form.cleaned_data['username']
            role = form.cleaned_data['role']

            if User.objects.all().filter(username = uname).count() == 0:
                return redirect('participants',course_id)
            U = User.objects.get(username = uname)
            if role == '1':
                if not U in c.students.all():
                    c.students.add(U)
            elif role == '2':
                if not U in c.TAs.all():
                    c.TAs.add(U)
                    P = TA_Permission.objects.create(user = U,course = c)
                    P.save()
            return redirect('participants',course_id)
    else:
        form = UserRemoveForm()


    IsINS = 'NO'
    if uname == c.instructor.username:
        IsINS = 'YES'

    TA_Perm = 'NO'
    if uname in Tnames:
        Q = TA_Permission.objects.all().filter(user = request.user,course = c)
        P = Q[0]
        if P.add_students == 'YES':
            TA_Perm = 'YES'

    args = {
        'course':   c,
        'Students': c.students.all(),
        'TAs':  c.TAs.all(),
        'Ins':  c.instructor,
        'IsINS':    IsINS,
        'form': form,
        'TA_Perm': TA_Perm
    }
    return render(request,'courses/participants.html',args)

def remove(request,course_id,user_id):

    C = Course.objects.get(pk = course_id)

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    if uname != C.instructor.username:
        return redirect('CourseHome')  

    U = User.objects.get(pk = user_id)

    Stds = C.students.all()
    TAs = C.TAs.all()

    if U in Stds:
        C.students.remove(U)
    if U in TAs:
        C.TAs.remove(U)
        if TA_Permission.objects.all().filter(user = U,course = C).count() != 0:
            P = TA_Permission.objects.all().filter(user = U,course = C)[0]
            P.delete()
    
    return redirect('participants',course_id)


def file_feedback(request,course_id,assign_id):
    a = Assignment.objects.get(pk = assign_id)
    c = Course.objects.get(pk = course_id )

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    
    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in Tnames and uname != c.instructor.username:
        return redirect('CourseHome')

    give_feedback(c,a)
                
    return redirect('submissions',course_id,assign_id)

def give_feedback(course,assign):

    subs = assign.filesubmission_set.all()

    raw = assign.feedfile.read()
    data = raw.decode("utf-8")
    lines = []
    s = ''
    for i in range(len(data)):
        char = data[i]
        if(ord(char) != 10 and ord(char) != 13):
            s += char
        else:
            if len(s) != 0:
                lines.append(s)
            s = ''
    if len(s) != 0:
        lines.append(s)

    for line in lines:
        L = line.split(',',2)
        print(L)
        for sub in subs:
            if L[0] == sub.user.username:
                marks = int(L[1])
                cap = assign.maxmarks
                marks = min(cap,marks)

                sub.corrected = 'YES'
                sub.grade = marks

                sub.feedback = L[2].replace("##","\n")

                sub.save()

def Settings(request,course_id,user_id):
    c = Course.objects.get(pk = course_id )

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    if uname != c.instructor.username:
        return redirect('CourseHome')

    ta = User.objects.get(pk = user_id)
    Q = ta.ta_permission_set.all().filter(course = c)
    P = Q[0]

    if request.method == 'POST':
        form = TaPermissionForm(request.POST)
        if form.is_valid():
            a = form.cleaned_data['assign_create']
            f = form.cleaned_data['forum_create']
            u = form.cleaned_data['add_users']

            P.assign_create = 'YES' if a == '2' else 'NO'
            P.forum_create = 'YES' if f == '2' else 'NO'
            P.add_students = 'YES' if u == '2' else 'NO'
            P.save()

            return redirect('TA_settings',course_id,user_id)
    else:
        form = TaPermissionForm()

    args = {
        'course': c,
        'P': P,
        'ta': ta,
        'form': form
    }
    return render(request,'courses/settings.html',args)

#__________________________________________________***************
def grades(request,course_id):
    c = Course.objects.get(pk = course_id )

    if not request.user.is_authenticated:
        return redirect('login')

    uname = request.user.username
    
    Snames = [ s.username for s in c.students.all() ]
    Tnames = [ s.username for s in c.TAs.all() ]
    uname = request.user.username
    if uname not in (Tnames + Snames) and uname != c.instructor.username:
        return redirect('CourseHome')

    Anames = [ s.name for s in c.assignment_set.all() ]
    nowtime = datetime.datetime.now()
    if uname in Snames:
        Marks = []
        for assign in c.assignment_set.all():
            l = [assign]
            str = 'OVER' if (nowtime > assign.deadline) else 'RUNNING'
            l.append(str)
            Subnames = [sub.user.username for sub in assign.filesubmission_set.all() ]
            if uname in Subnames:
                mysub = assign.filesubmission_set.get(user = request.user)
                if mysub.corrected == 'YES':
                    l.append(mysub.grade)
                    l.append('GRADED')
                else:
                    l.append(0)
                    l.append('NOT GRADED')
            else:
                l.append(0)
                l.append('NOT SUBMITTED')
            frac = max(mysub.grade,0)/assign.maxmarks
            l.append(round(frac*100,2))
            l.append(round(frac*assign.weightage,2))
            Marks.append(l)
        m = 0
        t = 0
        for e in Marks:
            m += e[5]
            t += e[0].weightage


        percs = [m[4] for m in Marks]
        grades = [m[5] for m in Marks]
        weights = [m[0].weightage for m in Marks]
        
        #------------------------totals-------------
        plt.plot(grades,color = 'orange',marker = "o",markersize = 10)
        plt.xlabel('Assignments',fontsize = 15)
        plt.ylabel('Marks in the Assignment',fontsize = 14)
        plt.title('Marks in all Assignments',fontsize = 16)
        plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
        plt.xticks([i for i in range(len(Anames))],Anames)

        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        graphM = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

        #----------percs
        plt.plot(percs,color = 'darkviolet',marker = "o",markersize = 10)
        plt.xlabel('Assignments',fontsize = 15)
        plt.ylabel('% Marks in the Assignment',fontsize = 14)
        plt.title('Percentage Marks in all Assignments',fontsize = 16)
        plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
        plt.xticks([i for i in range(len(Anames))],Anames)

        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        graphP = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

        ZZ = cumulative_per_total(grades,percs,weights)
        #----------totals cumulative---------
        plt.plot(ZZ[0],color = 'deepskyblue',marker = "o",markersize = 10)
        plt.xlabel('Assignments',fontsize = 15)
        plt.ylabel('Course Total till that Assignment',fontsize = 14)
        plt.title('Progress of Course Total',fontsize = 16)
        plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
        plt.xticks([i for i in range(len(Anames))],Anames)

        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        HM = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

        #----------percs cumulative---------
        plt.plot(ZZ[1],color = 'fuchsia',marker = "o",markersize = 10)
        plt.xlabel('Assignments',fontsize = 15)
        plt.ylabel('Course % till that Assignment',fontsize = 14)
        plt.title('Progress of Course Total %',fontsize = 16)
        plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
        plt.xticks([i for i in range(len(Anames))],Anames)

        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=300)
        HP = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
        buf.close()

        plt.clf()

        Corr_data = corrected_data(c)
        Totals_C =  total_data(Corr_data,len(Anames),c)[1]


        mean = Totals_C[-3]
        sd = Totals_C[-2]

        print(mean)
        print(sd)
        print()
        WARN = 'NO'
        if ZZ[1][-1] < (mean - sd):
            WARN = 'YES'

        args = {
            'INSrTA': False,
            'course': c,
            'Marks': Marks,
            'nowtime': nowtime,
            'total': [round((m*100)/t,2),t,round(m,2)],
            'GM':graphM,
            'GP':graphP,
            'HM':HM,
            'HP':HP,
            'warn':WARN,
        }
        return render(request,'courses/grades.html',args)

    else:
        A = c.assignment_set.all()
        Anames = [ a.name for a in A ]
        
        Data = nice_data(c)
        Raw_data = raw_data(c)
        Corr_data = corrected_data(c)

        list_raw_data = Raw_data.tolist()
        Row_Data = Data.tolist()
        list_corr_data = Corr_data.tolist()

        nice = []
        for i in range(len(Snames)):
            row = [Snames[i]]
            row = row + Row_Data[i]
            nice.append(row)
        #print(nice)

        raw = []
        for i in range(len(Snames)):
            row = [Snames[i]]
            row = row + list_raw_data[i]

            cleaned = map(lambda x: '--' if x == -2 else x,row)
            raw.append(list(cleaned))
        #print(raw)

        corrected = []
        for i in range(len(Snames)):
            row = [Snames[i]]
            row = row + list_corr_data[i]
            corrected.append(row)
        #print(corrected)

        ZZ = total_data(Data,len(Anames),c,False)
        Indivs_U = ZZ[0]

        ZZ = total_data(Corr_data,len(Anames),c)
        Indivs_C = ZZ[0]
        Totals_C = ZZ[1]

        Cumulatives =[ [] for i in range(len(Snames)+4) ]
        for i in range(1,len(Anames)+1):
            ZZ = total_data(Corr_data,i,c)
            TT = ZZ[1]
            for j in range(len(Snames)+4):
                Cumulatives[j].append(TT[j])
            

        marks = Totals_C[0:len(Snames)]
        cap = int(Totals_C[-4])
        #------plotting-------#
        if True:
            ax = plt.figure().gca()
            ax.yaxis.set_major_locator(MaxNLocator(integer=True))
            plt.hist(marks,bins = max(int(cap/10),1),edgecolor = 'k',linewidth = 2,color='blue',alpha = 0.68,label='Marks')
            plt.xlabel('Whole Marks Range',fontsize = 15)
            plt.ylabel('Frequency| num of students',fontsize = 15)
            plt.title('Histogram of Course Totals, Mean',fontsize = 18)
            plt.xlim([-10,cap+10])
            plt.grid(axis='y', alpha=0.6)
            plt.axvline(np.array(marks).mean(), color= '#01153E', linestyle='dashed', linewidth=2.5,label = 'mean')
            ax.legend(loc='lower center', bbox_to_anchor=(0.5, 0.1),
                ncol=3, fancybox=True, shadow=True)

        

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            histTot = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

        if True:
            Stat_array = np.array(Indivs_C)

            #-------------------mean
            plt.plot(Stat_array[:,0],color = 'b',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Mean of each Assignment',fontsize = 14)
            plt.title('Comparison of MEAN of all assignments',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            graphAM = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #----------------std dev
            plt.plot(Stat_array[:,1],color = 'red',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Stand. deviation of each Assignment',fontsize = 14)
            plt.title('Comparison of ST DEV of all assignments',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            graphAS = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #---------------variance
            plt.plot(Stat_array[:,2],color = 'green',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Variance of each Assignment',fontsize = 14)
            plt.title('Comparison of VARIANCE of all assignments',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            graphAV = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #------------------all in a single graph.
            plt.plot(Stat_array[:,0],color = 'b',marker = "o",markersize = 10,label = 'Mean')
            plt.plot(Stat_array[:,1],color = 'r',marker = "o",markersize = 10,label = 'std dev')            
            plt.plot(Stat_array[:,2],color = 'g',marker = "o",markersize = 10,label = 'variance')
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Mean,SD,Var of each Assignment',fontsize = 14)
            plt.title('Comparison of ALL stats between assignments',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)
            plt.legend()

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            graphAll = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

        if True:
            cum_means = Cumulatives[-3]
            cum_stds = Cumulatives[-2]
            cum_vars = Cumulatives[-1]

            #-------------------mean
            plt.plot(cum_means,color = 'deepskyblue',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Class Avg till that point',fontsize = 14)
            plt.title('Progress of class avg',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            CumAM = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #----------------std dev
            plt.plot(cum_stds,color = 'orange',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Class SD till that point',fontsize = 14)
            plt.title('Progress of the class SD',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            CumAS = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #---------------variance
            plt.plot(cum_vars,color = 'darkviolet',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('class Variance till that point',fontsize = 14)
            plt.title('Progress of the class Variance',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            CumAV = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #------------------all in a single graph.
            plt.plot(cum_means,color = 'deepskyblue',marker = "o",markersize = 10,label = 'Mean')
            plt.plot(cum_stds,color = 'orange',marker = "o",markersize = 10,label = 'std dev')            
            plt.plot(cum_vars,color = 'darkviolet',marker = "o",markersize = 10,label = 'variance')
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('Class stats at that time',fontsize = 14)
            plt.title('Progress of class stats',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)
            plt.legend()

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            CumAll = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

            #--------------percentages--------------
            mm = cum_means
            ww = Cumulatives[-4]
            percs = [ (mm[i]*100/ww[i]) for i in range(len(Anames))]

            plt.plot(percs,color = 'fuchsia',marker = "o",markersize = 10)
            plt.xlabel('Assignments',fontsize = 15)
            plt.ylabel('class avg percentage at that point',fontsize = 14)
            plt.title('Progress of the class avg %',fontsize = 16)
            plt.grid(color='silver', linestyle='-', linewidth=1,axis = 'y')
            plt.xticks([i for i in range(len(Anames))],Anames)

            buf = BytesIO()
            plt.savefig(buf, format='png', dpi=300)
            CumPerc = base64.b64encode(buf.getvalue()).decode('utf-8').replace('\n', '')
            buf.close()

            plt.clf()

        args = {
            'INSrTA': True,
            'course': c,
            'NiceData': nice,
            'RawData': raw,
            'CorrData': corrected,
            'Anames': Anames,
            'Snames':Snames,
            'assigns':A,
            'IU':Indivs_U,
            'IC': Indivs_C,
            'TC': Totals_C,
            'Cumls': Cumulatives,
            'NumS': len(Snames),
            'histT':histTot,
            'graphAM':graphAM,
            'graphAS':graphAS,
            'graphAV':graphAV,
            'graphAll':graphAll,
            'CumAM':CumAM,
            'CumAS':CumAS,
            'CumAV':CumAV,
            'CumAll':CumAll,
            'CumPerc':CumPerc,
        }
        return render(request,'courses/grades.html',args)



#----------helper functions for mean, avgs etc...
def nice_data(course):
    A = course.assignment_set.all()
    C = course
    S = C.students.all()

    Snames = [ s.username for s in S ]
    Anames = [ a.name for a in A ]

    numS = len(S)
    numA = len(A)

    Data = np.zeros((numS,numA))

    for assign in A:
        ind1 = Anames.index(assign.name)
        for sub in assign.filesubmission_set.all():
            ind2 = Snames.index(sub.user.username)
            if sub.corrected == 'YES':
                Data[ind2][ind1] = sub.grade
    
    #print(Snames)
    #print(Data)
    return Data

def raw_data(course):
    A = course.assignment_set.all()
    C = course
    S = C.students.all()

    Snames = [ s.username for s in S ]
    Anames = [ a.name for a in A ]

    numS = len(S)
    numA = len(A)

    Data = np.full((numS,numA),-2.0)

    for assign in A:
        ind1 = Anames.index(assign.name)
        for sub in assign.filesubmission_set.all():
            ind2 = Snames.index(sub.user.username)
            Data[ind2][ind1] = sub.grade
    
    #print(Snames)
    #print(Data)
    return Data

def corrected_data(course):
    A = course.assignment_set.all()
    C = course
    S = C.students.all()

    Snames = [ s.username for s in S ]
    Anames = [ a.name for a in A ]

    numS = len(S)
    numA = len(A)

    Data = np.zeros((numS,numA))

    for assign in A:
        ind1 = Anames.index(assign.name)
        for sub in assign.filesubmission_set.all():
            ind2 = Snames.index(sub.user.username)
            if sub.corrected == 'YES':
                Data[ind2][ind1] = round( ((sub.grade)*(assign.weightage)/assign.maxmarks), 2)
    
    #print(Snames)
    #print(Data)
    return Data

def total_data(Corr_Data,n,course,adjusted= True):
    A = course.assignment_set.all()
    C = course
    S = C.students.all()

    Snames = [ s.username for s in S ]
    Anames = [ a.name for a in A ]

    numS = len(S)
    numA = len(A)

    sliced = Corr_Data[:,0:n]

    means = np.mean(sliced,axis=0)
    stds = np.std(sliced,axis=0)
    vars = np.var(sliced,axis=0)
    totals = np.sum(sliced,axis=1)

    Indivs = []
    outof = 0
    for i in range(n):
        l_nr = [means[i],stds[i],vars[i]]
        l = [round(x,2) for x in l_nr ]
        outof += (A[i].weightage)
        if adjusted:
            l.append(A[i].weightage)
        else:
            l.append(A[i].maxmarks)
        Indivs.append(l)
    
    total_mean = np.mean(totals)
    total_std = np.std(totals)
    total_var = np.var(totals)

    Totals_nr =  totals.tolist() + [outof, total_mean,total_std,total_var]
    Totals = [round(x,2) for x in Totals_nr]



    return [Indivs,Totals]




def assign_marks_array(course,assign):
    A = assign
    C = course
    S = C.students.all()

    Snames = [ s.username for s in S ]

    numS = len(S)

    Data = np.zeros(numS)

    for sub in A.filesubmission_set.all():
        ind = Snames.index(sub.user.username)
        if sub.corrected == 'YES':
            Data[ind] = sub.grade
    
    #print(Snames)
    #print(Data)
    return Data

def cumulative_per_total(totals,percs,weights):
    n = len(totals)
    cum_total = [0 for i in range(n)]
    cum_weights = [0 for i in range(n)]
    for i in range(n):
        if i == 0:
            cum_total[i] = totals[0]
            cum_weights[i] = weights[0]
        else:
            cum_total[i] = cum_total[i-1] + totals[i]
            cum_weights[i] = cum_weights[i-1] + weights[i]

    T = cum_total
    W = cum_weights
    cum_percs = [ (T[i]/W[i])*100 for i in range(n)]

    return[T,cum_percs]


#________________________________________________**********
def alert(request,course_id):
    pass

#____________________________________________________**********
def autograde(request,course_id,assign_id):
    pass
