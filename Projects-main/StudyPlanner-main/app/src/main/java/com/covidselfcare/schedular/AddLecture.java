package com.covidselfcare.schedular;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class AddLecture extends AppCompatActivity {

    private EditText name,discription;
    private Button save;

    Datastorage db=new Datastorage(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_lecture);

        name=findViewById(R.id.lecturename);
        discription=findViewById(R.id.lecture);
        save=findViewById(R.id.btn_savelecture);

        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String nametxt=name.getText().toString();
                String discriptiontxt=discription.getText().toString();

                Boolean check=db.addlecture(nametxt,discriptiontxt);
                if(check==true){
                    Toast.makeText(AddLecture.this, "Saved the lecture", Toast.LENGTH_SHORT).show();
                    Intent intent=new Intent(AddLecture.this,MainActivity.class);

                    startActivity(intent);
                }
                else
                    Toast.makeText(AddLecture.this, "Not saved", Toast.LENGTH_SHORT).show();

            }
        });

    }
}