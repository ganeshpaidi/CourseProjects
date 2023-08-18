package com.covidselfcare.schedular;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class AddExam extends AppCompatActivity {

    private EditText name, discription, quizdate;
    private Button save;
    Datastorage db = new Datastorage(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_exam);


        name = findViewById(R.id.examname);
        discription = findViewById(R.id.examdiscription);
        quizdate = findViewById(R.id.examdate);
        save = findViewById(R.id.examsave);

        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String nametxt = name.getText().toString();
                String discriptiontxt = discription.getText().toString();
                int quizdateint;
                try {
                    quizdateint = Integer.parseInt(quizdate.getText().toString());
                } catch (Exception e) {
                    quizdateint = 0;
                }

                Boolean check = db.addexam(nametxt, discriptiontxt, quizdateint);
                if (check == true) {
                    Toast.makeText(AddExam.this, "Saved the Quiz", Toast.LENGTH_SHORT).show();
                    Intent intent;
                    intent = new Intent(AddExam.this, MainActivity.class);
                    startActivity(intent);
                } else {
                    Toast.makeText(AddExam.this, "Not saved", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
}