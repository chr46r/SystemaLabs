package com.example.mvpwallet

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.*
import java.io.OutputStream
import java.net.Socket
import java.io.IOException
import kotlin.concurrent.thread

class MainActivity : AppCompatActivity() {

    private lateinit var senderField: EditText
    private lateinit var receiverField: EditText
    private lateinit var amountField: EditText
    private lateinit var sendTxButton: Button
    private lateinit var responseView: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        senderField   = findViewById(R.id.senderField)
        receiverField = findViewById(R.id.receiverField)
        amountField   = findViewById(R.id.amountField)
        sendTxButton  = findViewById(R.id.sendTxButton)
        responseView  = findViewById(R.id.responseView)

        sendTxButton.setOnClickListener {
            val s = senderField.text.toString().trim()
            val r = receiverField.text.toString().trim()
            val a = amountField.text.toString().trim()

            if (s.isEmpty() || r.isEmpty() || a.isEmpty()) {
                Toast.makeText(this, "Please fill all fields", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            thread {
                var result = ""
                try {
                    // "10.0.2.2" for connecting to local PC from Android emulator
                    Socket("10.0.2.2", 5001).use { sock ->
                        val out: OutputStream = sock.getOutputStream()
                        val message = "TX $s,$r,$a"
                        out.write(message.toByteArray())
                        out.flush()
                    }
                    result = "Transaction sent successfully."
                } catch (e: IOException) {
                    result = "Error: ${e.message}"
                }

                runOnUiThread {
                    responseView.text = result
                }
            }
        }
    }
}
