using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class ArduinoCom : MonoBehaviour {
    private float amoutToMove;
    private string received = "A000B000C000D0000E0000F0000G00H00I00J00K";

    SerialPort serialPort = new SerialPort("COM4", 57600);

    // Use this for initialization
    void Start () {
        try
        {
            serialPort.Open();
        }
        catch(System.Exception)
        {
            // TODO
        }
	}
	
	// Update is called once per frame
	void Update () {
        if (serialPort.IsOpen)
        {
            serialPort.WriteLine("N");
            received = serialPort.ReadTo("K");
        }
        amoutToMove = int.Parse(received.Substring(13, 4)) * Time.deltaTime;
        
        transform.Translate(Vector3.left * amoutToMove, Space.World);
	}
}
