using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Handles enemy aiming, firing, and death mechanics.
/// </summary>
public class enemyControl : MonoBehaviour
{
    public Rigidbody shell;
    public Transform firePosition;
    public ParticleSystem tankExplosion;
    public AudioClip explosionAudio;
    private float launchForce = 20f;

    private GameObject player;
    private Rigidbody rigidBody;
    private GameObject cannon;
    private GameObject barrel;
    private GameObject tankAudio;

    private float cannonSpeed = 60f;
    private float cannonAngle = 0f;

    private bool reloading = false;
    private int health = 3;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        cannon = gameObject.transform.GetChild(2).gameObject;
        barrel = cannon.transform.GetChild(0).gameObject;
        rigidBody = GetComponent<Rigidbody>();
        tankAudio = gameObject.transform.GetChild(4).gameObject;

        rigidBody.isKinematic = true;
    }

    // Update is called once per frame
    void Update()
    {
        checkPlayer();
    }

    /// <summary>
	/// Constantly checks if the player is within range. Uses the hypotenuse of x and z to calculate distance
	/// and then checks if they are within an angle of 40 degrees. If so, they begin aiming and firing based
	/// on position and distance.
	/// </summary>
    private void checkPlayer()
    {
        Vector3 distance = player.transform.position - transform.position;
        float hypotenuse = Mathf.Sqrt(Mathf.Pow(distance.x, 2) + Mathf.Pow(distance.z, 2));
        float angle = Vector3.Angle(distance, transform.forward);


        // If hypotenuse is less than 60f away and within 40 degrees
        if (hypotenuse < 60f && angle < 40f)
        {
            // Aim cannon and fire
            cannon.transform.LookAt(player.transform);
            fire();

            // Aim more up depending on distance from player tank
            if (hypotenuse > 0f && hypotenuse <= 10f)
            {
                raiseCannon(0f);
            }
            else if (hypotenuse > 11f && hypotenuse <= 20f)
            {
                raiseCannon(-5);
            }
            else if (hypotenuse > 21f && hypotenuse <= 30f)
            {
                raiseCannon(-10);
            }
            else if (hypotenuse > 31f && hypotenuse <= 40f)
            {
                raiseCannon(-15);
            }
            else if (hypotenuse > 41f && hypotenuse <= 50f)
            {
                raiseCannon(-25);
            }
            else if (hypotenuse > 51f && hypotenuse <= 60f)
            {
                raiseCannon(-35);
            }
        }
    }

    /// <summary>
	/// Checks if reloading, and if not, will fire a shell from the cannon
	/// barrel.
	/// </summary>
    private void fire()
    {
        if (reloading == false)
        {
            reloading = true;
            Rigidbody shellInstance = Instantiate(shell, firePosition.position, firePosition.rotation) as Rigidbody;
            shellInstance.velocity = launchForce * firePosition.forward;
            StartCoroutine(reload());
        }
    }

    /// <summary>
	/// Raises the cannon based on the angle provided.
	/// </summary>
	/// <param name="maxAngle">Continues to raise the cannon to this angle.</param>
    private void raiseCannon(float maxAngle)
    {
        if (maxAngle < cannonAngle)
        {
            barrel.transform.localEulerAngles = new Vector3(barrel.transform.localEulerAngles.x - (cannonSpeed * Time.deltaTime), 0f, 0f);
            cannonAngle -= cannonSpeed * Time.deltaTime;
        }
        else if (maxAngle > cannonAngle)
        {
            barrel.transform.localEulerAngles = new Vector3(barrel.transform.localEulerAngles.x + (cannonSpeed * Time.deltaTime), 0f, 0f);
            cannonAngle += cannonSpeed * Time.deltaTime;
        }
    }

    /// <summary>
	/// If enemy is hit with a shell, decrement health. If health reaches 0, detonate
	/// the tank and remove the game object.
	/// </summary>
	/// <param name="collision"></param>
    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "shell")
        {
            // Decrement health
            health--;

            // If health has reached 0
            if (health == 0)
            {
                // Detach particle system and audio, play both
                player.GetComponent<enemyCount>().decrement();
                tankExplosion.transform.parent = null;
                tankExplosion.Play();

                tankAudio.transform.parent = null;
                tankAudio.GetComponent<AudioSource>().PlayOneShot(explosionAudio);

                // Destroy game object and detached game objects
                Destroy(tankAudio.gameObject, explosionAudio.length);
                Destroy(tankExplosion.gameObject, tankExplosion.main.duration);
                Destroy(gameObject);
            }
        }
    }

    /// <summary>
	/// Wait 3 seconds, then reset the reloading boolean.
	/// </summary>
	/// <returns></returns>
    IEnumerator reload()
    {
        yield return new WaitForSeconds(3);
        reloading = false;
    }
}
