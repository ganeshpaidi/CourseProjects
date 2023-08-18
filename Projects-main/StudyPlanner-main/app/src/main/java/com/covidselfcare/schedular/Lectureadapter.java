package com.covidselfcare.schedular;

import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class Lectureadapter extends RecyclerView.Adapter<Lectureadapter.myviewholder>{

    ArrayList<Model_lecture> dataholder;
    Datastorage db;
    //Context context;

    public Lectureadapter(ArrayList<Model_lecture> dataholder, Datastorage db) {
        this.dataholder = dataholder;
        this.db=db;
        //this.context=context;
    }

    @NonNull
    @Override
    public myviewholder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.item_lecture,parent,false);
        return new myviewholder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull myviewholder holder, int position) {
        holder.dname.setText(dataholder.get(position).getName());
        holder.dplan.setText(dataholder.get(position).getDiscription());
        holder.ddatetime.setText(dataholder.get(position).getCreatedtime());
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
            dname=(TextView) itemView.findViewById(R.id.displaylecturename);
            dplan=(TextView) itemView.findViewById(R.id.displaylecture);
            ddatetime=(TextView) itemView.findViewById(R.id.displaylecturedate);
            mdelete=(ImageView) itemView.findViewById(R.id.del_lecture);

            dplan.setMovementMethod(new ScrollingMovementMethod());

            mdelete.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    final Boolean aBoolean = db.del_lecture(dataholder.get(getAdapterPosition()).getId());
                    dataholder.remove(getAdapterPosition());
                    notifyItemRemoved(getAdapterPosition());
                }
            });
        }
    }
}


