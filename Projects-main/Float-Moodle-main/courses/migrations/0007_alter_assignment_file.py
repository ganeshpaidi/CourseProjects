# Generated by Django 3.2.8 on 2021-10-19 15:17

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('courses', '0006_alter_assignment_file'),
    ]

    operations = [
        migrations.AlterField(
            model_name='assignment',
            name='file',
            field=models.FileField(blank=True, default='settings.MEDIA_ROOT/foo/FloatMoodle.pdf', upload_to='assignments'),
        ),
    ]