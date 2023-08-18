package com.covidselfcare.schedular;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class AddStudy extends AppCompatActivity {

    private EditText name, plan;
    private Button save;
    Datastorage db=new Datastorage(this);;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_study);


        name=findViewById(R.id.Enter_planname);
        plan=findViewById(R.id.Enter_studyplan);
        save=findViewById(R.id.btn_savestudyplan);

        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String nametxt=name.getText().toString();
                String plantxt=plan.getText().toString();


                Boolean checkinsertdata=db.addstudyplan(nametxt,plantxt);
                if(checkinsertdata==true){
                    Toast.makeText(AddStudy.this, "Saved the plan", Toast.LENGTH_SHORT).show();
                    Intent intent;
                    intent = new Intent(AddStudy.this,MainActivity.class);
                    startActivity(intent);

                }else {
                    Toast.makeText(AddStudy.this, "Not saved", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

}