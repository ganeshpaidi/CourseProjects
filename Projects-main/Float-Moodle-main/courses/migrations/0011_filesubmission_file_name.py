# Generated by Django 3.2.8 on 2021-10-19 20:02

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('courses', '0010_filesubmission_corrected'),
    ]

    operations = [
        migrations.AddField(
            model_name='filesubmission',
            name='file_name',
            field=models.CharField(default='submission', max_length=100),
        ),
    ]
