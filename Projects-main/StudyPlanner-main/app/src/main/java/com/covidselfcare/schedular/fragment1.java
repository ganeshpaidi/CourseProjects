package com.covidselfcare.schedular;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class fragment1 extends Fragment {


    private Button newPlan;

    RecyclerView recyclerView;
    ArrayList<Model_studyplan> dataholder = new ArrayList<Model_studyplan>();
    //DBHelper db=new DBHelper(getContext());

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view= inflater.inflate(R.layout.studyplan_layout,container,false);

        newPlan=view.findViewById(R.id.add_studyplan);
        recyclerView=(RecyclerView)view.findViewById(R.id.recview_plans);
        recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        dataholder.clear();

        Datastorage db= new Datastorage(getActivity());
        Cursor cursor=db.readAllplans();

        while (cursor.moveToNext()){
            Model_studyplan obj = new Model_studyplan(cursor.getInt(0), cursor.getString(1),cursor.getString(2),cursor.getString(3));
            dataholder.add(obj);
        }
        Studyplanadapter adapter=new Studyplanadapter(dataholder,db);
        recyclerView.setAdapter(adapter);

        newPlan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getActivity(),AddStudy.class);
                startActivity(intent);
            }
        });


        return view;
    }
}
