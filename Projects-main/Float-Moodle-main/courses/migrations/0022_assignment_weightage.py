# Generated by Django 3.2.8 on 2021-11-27 21:01

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('courses', '0021_alter_forum_course'),
    ]

    operations = [
        migrations.AddField(
            model_name='assignment',
            name='weightage',
            field=models.IntegerField(default=0),
        ),
    ]
