from django import forms

#Default form date time field is type='text'
#Now it renders as a type='datetime-local' in HTML.
class DateTimeInput(forms.DateTimeInput):
    input_type = 'datetime-local'


class CourseCreationForm(forms.Form):
    name = forms.CharField(
        label='Course name',
        max_length=100,
        widget = forms.TextInput(attrs = {'class' : 'textareaform'})
    )

class CourseJoinForm(forms.Form):
    code = forms.CharField(label='Enter Course Code', max_length=10)

class AssignCreationForm(forms.Form):
    name = forms.CharField(label='Assignment name', max_length=50,required=True)
    file = forms.FileField(label='Problem File',required=False)
    #file_name = forms.CharField(label='FileName', max_length=50)
    weblink = forms.URLField(label='URL link',required=False)
    statement = forms.CharField(
            widget= forms.Textarea(
                attrs={'style': 'border-color: orange;' 'width: 80%' 'height: 200px' 'padding: 12px 20px;' 'border: 2px solid #ccc;' 'border-radius: 15px;''background-color: #f8f8f8;' }
            ),
            label='Description',
            required=True,
            max_length=300
    )
    marks = forms.IntegerField(label='Max Marks',required=True)
    weightage = forms.IntegerField(label='Weightage',required=True)
    deadline = forms.DateTimeField( #using the calender-time selection field of HTML.
            widget= DateTimeInput(),
            label='Deadline',
            required=True
    )


class AssignmentSubmitForm(forms.Form):
    file = forms.FileField(label='Submission File',required=True)

class FeedbackForm(forms.Form):
    feedback = forms.CharField(
        widget= forms.Textarea(
            attrs={'style': 'border-color: orange;' 'width: 80%;' 'height: 400px' 'padding: 12px 20px;' 'border: 2px solid #ccc;' 'border-radius: 15px;''background-color: #f8f8f8;' }
        ),
        label='Feedback',
        required=True,
        max_length=300
    )
    grade = forms.IntegerField(label='Grade',required=True)

class ForumCreateForm(forms.Form):
    name = forms.CharField(
        widget = forms.TextInput(
            attrs= {'style': 'border-color: orange;' 'width: 90%;' 'height: 10%' 'padding: 10px 20px;' 'border: 2px solid #ccc;' 'border-radius: 15px;''background-color: #f8f8f8;' }
        ),
        label = 'Forum Name',
        required=True,
        max_length=20
    )

CHOICES =(
    ("1", "STUDENT"),
    ("2", "TA"),
)

PERMISNS =(
    ("1", "NO"),
    ("2", "YES"),
)


class UserRemoveForm(forms.Form):
    username = forms.CharField(
        widget = forms.TextInput(
            attrs= {'style': 'border-color: orange;' 'width: 50%;' 'height: 20%' 'padding: 10px 20px;' 'border: 2px solid #ccc;' 'border-radius: 15px;''background-color: #81b0e0;' }
        ),
        label = 'username',
        required=True,
        max_length=50
    )
    role = forms.ChoiceField(choices = CHOICES)

class TaPermissionForm(forms.Form):
    assign_create = forms.ChoiceField(choices=PERMISNS,label='Create Assignment',required=True)
    forum_create = forms.ChoiceField(choices=PERMISNS,label='Create Forum',required=True)
    add_users = forms.ChoiceField(choices=PERMISNS,label='Add Users',required=True)

