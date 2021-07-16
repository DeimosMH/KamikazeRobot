using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMoveZoom : MonoBehaviour
{
    float speed = 10.0f;


    // Start is called before the first frame update
    private void Start()
    {

    }

    // Update is called once per frame
    private void Update()
    {

        Arrows();

    }


   

    public void Arrows()
    {
        //transform.position += new Vector3(((Input.GetAxisRaw("right") * Time.deltaTime * speed)),OR(Input.GetAxisRaw("right") * Time.deltaTime * speed))Input.GetAxisRaw("Mouse Y") * Time.deltaTime * speed);
        if (Input.GetKey("up"))
        {
            transform.position += new Vector3( 0, (2 * Time.deltaTime * speed));
        }

        if (Input.GetKey("down"))
        {
            transform.position += new Vector3(0, (2 * Time.deltaTime * -speed));
        }

        if (Input.GetKey("right"))
        {
            transform.position += new Vector3((2 * Time.deltaTime * speed), 0);
        }

        if (Input.GetKey("left"))
        {
            transform.position += new Vector3((2 * Time.deltaTime * -speed), 0);
        }
    }
}
