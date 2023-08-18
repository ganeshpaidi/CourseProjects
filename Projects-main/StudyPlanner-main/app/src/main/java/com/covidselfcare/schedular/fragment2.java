package com.covidselfcare.schedular;


import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class fragment2 extends Fragment {

    private Button newass;

    RecyclerView recyclerView;
    ArrayList<Model_assignment> dataholder= new ArrayList<Model_assignment>();


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.assignment_layout,container,false);

        newass=view.findViewById(R.id.add_ass);

        recyclerView=(RecyclerView) view.findViewById(R.id.recview_ass);
        recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        dataholder.clear();

        Datastorage db= new Datastorage(getActivity());
        Cursor cursor=db.readAllassignment();

        while(cursor.moveToNext()){
            Model_assignment obj= new Model_assignment(cursor.getInt(0),cursor.getString(1),cursor.getString(2),cursor.getInt(3),cursor.getString(4));
            dataholder.add(obj);
        }

        Assignmentadapter adapter=new Assignmentadapter(dataholder,db);

        recyclerView.setAdapter(adapter);












        newass.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getActivity(),AddAssignment.class);
                startActivity(intent);
            }
        });









        return view;
    }
}
