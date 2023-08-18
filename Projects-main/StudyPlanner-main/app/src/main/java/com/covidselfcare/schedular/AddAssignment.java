package com.covidselfcare.schedular;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class AddAssignment extends AppCompatActivity {

    private EditText name,discription, duedate;
    private Button save;
    Datastorage db=new Datastorage(this);


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_assignment);

        name=findViewById(R.id.assignmentname);
        discription=findViewById(R.id.assignmemntdiscription);
        duedate=findViewById(R.id.assignmentduedate);
        save=findViewById(R.id.asssave);

        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String nametxt=name.getText().toString();
                String discriptiontxt=discription.getText().toString();
                int duedateint;
                try {
                    duedateint=Integer.parseInt(duedate.getText().toString());
                }
                catch (Exception e){
                    duedateint=0;
                }

                Boolean check=db.addassignment(nametxt,discriptiontxt,duedateint);
                if(check==true){
                    Toast.makeText(AddAssignment.this, "Saved the assignment", Toast.LENGTH_SHORT).show();
                    Intent intent;
                    intent = new Intent(AddAssignment.this,MainActivity.class);
                    startActivity(intent);
                }
                else
                    Toast.makeText(AddAssignment.this, "Not saved the assignment", Toast.LENGTH_SHORT).show();

            }
        });
    }
}