using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Handles the camera switch. If "f" is pressed, switch from camera 1
/// to 2 and vice versa.
/// </summary>
public class cameraSwitch : MonoBehaviour
{
    private Camera cam1;
    private Camera cam2;
    // Start is called before the first frame update
    void Start()
    {
        cam1 = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<Camera>();
        cam2 = GameObject.FindGameObjectWithTag("cameraTwo").GetComponent<Camera>();
        cam1.enabled = true;
        cam2.enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        // If f is pressed, switch cameras
        if (Input.GetKeyDown("f"))
        {
            cam1.enabled = !cam1.enabled;
            cam2.enabled = !cam2.enabled;
        }
    }
}
