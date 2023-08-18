package com.covidselfcare.schedular;

import android.database.Cursor;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.util.Log;
import android.widget.CalendarView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class CalenderActivity extends AppCompatActivity {

    private CalendarView c;
    private ArrayList<Integer> arrayList;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.calender);
        Datastorage db= new Datastorage(getBaseContext());
        c = (CalendarView)findViewById(R.id.calender);
        TextView types[] = {findViewById(R.id.value0),findViewById(R.id.value1),findViewById(R.id.value2),findViewById(R.id.value3)};
        c.setOnDateChangeListener(new CalendarView.OnDateChangeListener() {
            @Override
            public void onSelectedDayChange(@NonNull CalendarView calendarView, int i, int i1, int i2) {
                i1++;
                String date1="",date2="";
                if(i2<10){
                    date1+="0";
                    date2+="0";
                }
                date1+=i2;date2+=i2+":";
                if(i1<10){
                    date1+="0";
                    date2+="0";
                }
                date1+=i1;date2+=i1+":";
                date1+=i;date2+=i;
                Log.d("Date : ","Date is " + date1+" "+date2);
                arrayList = db.getEvents(date1,date2);
                Log.d("Val:", String.valueOf(arrayList));
                for(int index=0;index<4;index++){
                    types[index].setText(String.valueOf(arrayList.get(index)));
                }
            }
        });
    }
}
