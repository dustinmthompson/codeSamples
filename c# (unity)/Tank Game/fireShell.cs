using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Handles the player's fire and shell mechanics.
/// </summary>
public class fireShell : MonoBehaviour
{
    public Rigidbody shell;
    public Transform firePosition;
    private float launchForce = 20f;
    private bool reloading = false;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        // If "space" is pressed down, fire a shell
        if (Input.GetKeyDown("space"))
        {
            fire();
        }
    }

    /// <summary>
	/// Fires a player shell. "Reloads" for 1 second, then the player
	/// can fire again.
	/// </summary>
    private void fire()
    {
        // If not reloading, fire a shell and start reloading
        if (reloading == false)
        {
            reloading = true;
            Rigidbody shellInstance = Instantiate(shell, firePosition.position, firePosition.rotation) as Rigidbody;
            shellInstance.velocity = launchForce * firePosition.forward;
            StartCoroutine(reload());
        }
    }

    /// <summary>
	/// Waits one full second then resets reloading boolean
	/// </summary>
	/// <returns></returns>
    IEnumerator reload()
    {
        yield return new WaitForSeconds(1);
        reloading = false;
    }
}
