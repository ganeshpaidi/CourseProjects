package com.covidselfcare.schedular;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class Studyplanadapter extends RecyclerView.Adapter<Studyplanadapter.myviewholder>{

    ArrayList<Model_studyplan> dataholder;
    Datastorage db;
    //Context context;

    public Studyplanadapter(ArrayList<Model_studyplan> dataholder,Datastorage db) {
        this.dataholder = dataholder;
        this.db=db;
        //this.context=context;
    }

    @NonNull
    @Override
    public myviewholder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.item_studyplan,parent,false);
        return new myviewholder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull myviewholder holder, int position) {
        holder.dname.setText(dataholder.get(position).getName());
        holder.dplan.setText(dataholder.get(position).getMyplan());
        holder.ddatetime.setText(dataholder.get(position).getDatetime());
    }

    @Override
    public int getItemCount() {

        return dataholder.size();
    }

    class myviewholder extends RecyclerView.ViewHolder{

        TextView dname,dplan,ddatetime;
        ImageView mdelete;

        public myviewholder(@NonNull View itemView) {
            super(itemView);
            dname=(TextView) itemView.findViewById(R.id.displayplanname);
            dplan=(TextView) itemView.findViewById(R.id.displayplan);
            ddatetime=(TextView) itemView.findViewById(R.id.displayplandate);
            mdelete=(ImageView) itemView.findViewById(R.id.del_plan);

            dplan.setMovementMethod(new ScrollingMovementMethod());

            mdelete.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    final Boolean aBoolean = db.del_plan(dataholder.get(getAdapterPosition()).getId());
                    dataholder.remove(getAdapterPosition());
                    notifyItemRemoved(getAdapterPosition());
                }
            });
        }
    }
}

