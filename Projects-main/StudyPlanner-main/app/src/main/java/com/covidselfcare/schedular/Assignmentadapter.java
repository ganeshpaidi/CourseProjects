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

public class Assignmentadapter extends RecyclerView.Adapter<Assignmentadapter.myviewholder>{

    ArrayList<Model_assignment> dataholder;
    Datastorage db;

    public Assignmentadapter(ArrayList<Model_assignment> dataholder, Datastorage db) {
        this.dataholder = dataholder;
        this.db = db;
    }

    @NonNull
    @Override
    public myviewholder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(parent.getContext()).inflate(R.layout.item_assignment,parent,false);
        return new myviewholder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull myviewholder holder, int position) {
        holder.dname.setText(dataholder.get(position).getName());
        holder.ddiscription.setText(dataholder.get(position).getDiscription());
        holder.dduedate.setText(dataholder.get(position).getDuetime());
    }

    @Override
    public int getItemCount() {
        return dataholder.size();
    }

    class myviewholder extends RecyclerView.ViewHolder{

        TextView dname,ddiscription,dduedate;
        ImageView mdelete;

        public myviewholder(@NonNull View itemView) {
            super(itemView);

            dname=(TextView) itemView.findViewById(R.id.displayassname);
            ddiscription=(TextView)itemView.findViewById(R.id.displayass);
            dduedate=(TextView)itemView.findViewById(R.id.displayassdate);
            mdelete=(ImageView) itemView.findViewById(R.id.del_ass);

            ddiscription.setMovementMethod(new ScrollingMovementMethod());

            mdelete.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    final Boolean abool=db.del_assignment(dataholder.get(getAdapterPosition()).getId());
                    dataholder.remove(getAdapterPosition());
                    notifyItemRemoved(getAdapterPosition());
                }
            });
        }
    }
}
