package com.covidselfcare.schedular;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.Locale;
import java.util.Set;

public class Datastorage extends SQLiteOpenHelper {

    public Datastorage(Context context) {
        super(context, "Education.db", null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase sqLiteDatabase) {
        sqLiteDatabase.execSQL("create Table StudyPlan(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT, myplan TEXT,datetime TEXT)");
        sqLiteDatabase.execSQL("create Table Assignments(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT, discription TEXT, duedate INTEGER , createdtime TEXT)");
        sqLiteDatabase.execSQL("create Table Exams(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT, discription TEXT, quizdate INTEGER, createdtime TEXT)");
        sqLiteDatabase.execSQL("create Table Lectures(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT, discription TEXT, createdtime TEXT)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {
        sqLiteDatabase.execSQL("DROP TABLE IF EXISTS StudyPlan");
        sqLiteDatabase.execSQL("DROP TABLE IF EXISTS Assignments");
        sqLiteDatabase.execSQL("DROP TABLE IF EXISTS Exams");
        sqLiteDatabase.execSQL("DROP TABLE IF EXISTS Lectures");
        onCreate(sqLiteDatabase);
    }

    public boolean addstudyplan(String name, String studyplan){
        SQLiteDatabase db=this.getWritableDatabase();
        ContentValues contentValues=new ContentValues();
        contentValues.put("name",name);
        contentValues.put("myplan",studyplan);

        SimpleDateFormat sdf = new SimpleDateFormat("dd:MM:yyyy", Locale.getDefault());
        String datetime = sdf.format(new Date());
        contentValues.put("datetime",datetime);

        long result=db.insert("StudyPlan",null,contentValues);
        if (result==-1){
            return false;
        }else {
            return true;
        }

    }

    public Cursor readAllplans(){
        SQLiteDatabase db= this.getReadableDatabase();
        String qry="select * from StudyPlan";
        Cursor cursor=db.rawQuery(qry,null);
        return cursor;
    }

    public ArrayList<Integer> getEvents(String date, String datef){
        SQLiteDatabase db= this.getReadableDatabase();
        Cursor cursorA=db.rawQuery("select * from Assignments where duedate = ? ",new String[]{date});
        Cursor cursorE=db.rawQuery("select * from Exams where quizdate = ? ",new String[]{date});
        Cursor cursorL=db.rawQuery("select * from Lectures where createdtime = ? ",new String[]{datef});
        Cursor cursorS=db.rawQuery("select * from StudyPlan where datetime = ? ",new String[]{datef});
        ArrayList<Integer> arrayList = new ArrayList<Integer>();
        arrayList.add(0,cursorL.getCount());
        arrayList.add(1,cursorS.getCount());
        arrayList.add(2,cursorE.getCount());
        arrayList.add(3,cursorA.getCount());
        return arrayList;
    }

    public boolean del_plan(int id){
        String i=String.valueOf(id);
        SQLiteDatabase db= this.getWritableDatabase();
        Cursor cursor=db.rawQuery("select * from StudyPlan where id = ?",new String[]{i});
        if (cursor.getCount()>0){
            long result=db.delete("StudyPlan", "id=?",new String[]{i});
            if(result==-1){
                return false;
            }
            else
                return true;
        }else
            return false;
    }

    public boolean addlecture(String name,String discription){
        SQLiteDatabase db=this.getWritableDatabase();
        ContentValues contentValues=new ContentValues();
        contentValues.put("name",name);
        contentValues.put("discription",discription);

        SimpleDateFormat sdf = new SimpleDateFormat("dd:MM:yyyy", Locale.getDefault());
        String createdtime=sdf.format(new Date());
        contentValues.put("createdtime",createdtime);
        Log.d("df",createdtime);
        long result=db.insert("Lectures",null,contentValues);
        if(result==-1){
            return false;
        }
        else
            return true;
    }

    public Cursor readAllectures(){
        SQLiteDatabase db=this.getReadableDatabase();
        String qry="select * from Lectures";
        Cursor cursor=db.rawQuery(qry,null);
        return cursor;
    }

    public boolean del_lecture(int id){
        String i=String.valueOf(id);
        SQLiteDatabase db=this.getWritableDatabase();
        Cursor cursor=db.rawQuery("select * from Lectures where id = ?", new String[]{i});
        if(cursor.getCount()>0){
            long result=db.delete("Lectures","id=?",new String[]{i});
            if(result==-1){
                return false;
            }
            else
                return true;
        }else
            return false;
    }

    public boolean addassignment(String name,String description, int duedate){
        SQLiteDatabase db=this.getWritableDatabase();
        ContentValues contentValues=new ContentValues();
        contentValues.put("name",name);
        contentValues.put("discription",description);
        contentValues.put("duedate",duedate);

        SimpleDateFormat sdf = new SimpleDateFormat("dd:MM:yyyy", Locale.getDefault());
        String createdtime=sdf.format(new Date());
        contentValues.put("createdtime",createdtime);

        long result=db.insert("Assignments",null,contentValues);
        if(result==-1){
            return false;
        }
        else
            return true;
    }

    public Cursor readAllassignment(){
        SQLiteDatabase db=this.getReadableDatabase();
        String qry="select * from Assignments";
        Cursor cursor=db.rawQuery(qry,null);
        return cursor;
    }

    public boolean del_assignment(int id){
        String i=String.valueOf(id);
        SQLiteDatabase db=this.getWritableDatabase();
        Cursor cursor=db.rawQuery("select * from Assignments where id = ?",new String[]{i});
        if(cursor.getCount()>0){
            long result=db.delete("Assignments","id=?",new String[]{i});
            if(result==-1){
                return false;
            }
            else
                return true;
        }
        else
            return false;
    }

    public boolean addexam(String name,String discription, int quizdate) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put("name", name);
        contentValues.put("discription", discription);
        contentValues.put("quizdate", quizdate);

        SimpleDateFormat sdf = new SimpleDateFormat("dd:MM:yyyy", Locale.getDefault());
        String createdtime = sdf.format(new Date());
        contentValues.put("createdtime", createdtime);

        long result = db.insert("Exams", null, contentValues);
        if (result == -1) {
            return false;
        } else
            return true;
    }

    public Cursor readAllexams(){
        SQLiteDatabase db= this.getReadableDatabase();
        String qry="select * from Exams";
        Cursor cursor=db.rawQuery(qry,null);
        return cursor;
    }

    public boolean del_exam(int id){
        String i=String.valueOf(id);
        SQLiteDatabase db=this.getWritableDatabase();
        Cursor cursor=db.rawQuery("select * from Exams where id = ?",new String[]{i});
        if(cursor.getCount()>0){
            long result=db.delete("Exams","id=?",new String[]{i});
            if(result==-1){
                return false;
            }
            else
                return true;
        }
        else
            return false;
    }
}

