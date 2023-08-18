package com.covidselfcare.schedular;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
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

public class fragment4 extends Fragment {

    private Button newlecture;
    RecyclerView recyclerView;

    ArrayList<Model_lecture> dataholder=new ArrayList<Model_lecture>();

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view= inflater.inflate(R.layout.lecture_layout,container,false);

        newlecture=view.findViewById(R.id.add_lecture);
        recyclerView=view.findViewById(R.id.recview_lectures);
        recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));

        dataholder.clear();

        Datastorage db=new Datastorage(getActivity());
        Cursor cursor=db.readAllectures();

        while (cursor.moveToNext()){
            Model_lecture obj=new Model_lecture(cursor.getInt(0),cursor.getString(1),cursor.getString(2),cursor.getString(3));
            Log.d("Lect",cursor.getString(3));
            dataholder.add(obj);
        }

        Lectureadapter adapter=new Lectureadapter(dataholder,db);
        recyclerView.setAdapter(adapter);




        newlecture.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getActivity(),AddLecture.class);
                startActivity(intent);
            }
        });






        return view;
    }
}
