using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

/// <summary>
/// Primary class of the program. Handles the player movement, aiming,
/// and player audio
/// </summary>
public class playerControl : MonoBehaviour
{
    public ParticleSystem tankExplosion;
    public GameObject camera1;
    public GameObject camera2;
    public AudioClip engineIdle;
    public AudioClip engineActive;
    private GameObject healthText;
    private GameObject canvas;
    private int health = 20;

    private float movementInput = 0f;
    private float movementSpeed = 10f;

    private float turnInput = 0f;
    private float turnSpeed = 30f;

    private float tireAngle = 0f;
    private float maxTireAngle = 45f;
    private float tireSpeed = 80f;

    private float cannonSpeed = 60f;
    private float cannonAngleH = 0f;
    private float maxCannonAngleH = 90f;
    private float cannonAngleV = 0f;
    private float maxCannonAngleV = -30f;

    private AudioSource engineAudio;
    private GameObject leftBackWheel;
    private GameObject leftFrontWheel;
    private GameObject rightBackWheel;
    private GameObject rightFrontWheel;
    private GameObject leftSteer;
    private GameObject rightSteer;
    private GameObject cannon;
    private GameObject barrel;

    private Rigidbody rigidBody;

    // Start is called before the first frame update
    void Start()
    {
        // Assign rigidbody
        rigidBody = GetComponent<Rigidbody>();
        rigidBody.isKinematic = false;

        // Assign components
        leftBackWheel = GameObject.FindGameObjectWithTag("leftBackWheel");
        leftFrontWheel = GameObject.FindGameObjectWithTag("leftFrontWheel");
        rightBackWheel = GameObject.FindGameObjectWithTag("rightBackWheel");
        rightFrontWheel = GameObject.FindGameObjectWithTag("rightFrontWheel");
        leftSteer = GameObject.FindGameObjectWithTag("leftSteer");
        rightSteer = GameObject.FindGameObjectWithTag("rightSteer");
        cannon = GameObject.FindGameObjectWithTag("cannon");
        barrel = GameObject.FindGameObjectWithTag("barrel");
        healthText = GameObject.FindGameObjectWithTag("health");
        canvas = GameObject.FindGameObjectWithTag("canvas");
        engineAudio = gameObject.GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        // Get inputs
        movementInput = Input.GetAxis("Vertical");
        turnInput = Input.GetAxis("Horizontal");

        // Reset game if "r" is pressed
        if (Input.GetKeyDown("r"))
        {
            resetGame();
        }
    }

    private void FixedUpdate()
    {
        move();
        turn();
        rotateWheels();
        rotateCannon();
    }

    /// <summary>
	/// Moves the tank forwards or backwards depending on input
	/// </summary>
    private void move()
    {
        // Convert input to movement
        Vector3 movement = transform.forward * movementInput * movementSpeed * Time.deltaTime;
        rigidBody.MovePosition(rigidBody.position + movement);
    }

    /// <summary>
	/// Turns the vehicle only when moving forwards or backwards
	/// </summary>
    private void turn()
    {
        // Turn only when moving
        if (movementInput > 0f)
        {
            float turn = turnInput * turnSpeed * Time.deltaTime;
            Quaternion turnRotation = Quaternion.Euler(0f, turn, 0f);
            rigidBody.MoveRotation(rigidBody.rotation * turnRotation);

            if (engineAudio.clip == engineIdle)
            {
                engineAudio.clip = engineActive;
                engineAudio.Play();
            }
        }
        // Invert turn direction when moving backwards
        else if (movementInput < 0f)
        {
            float turn = turnInput * turnSpeed * Time.deltaTime;
            Quaternion turnRotation = Quaternion.Euler(0f, -1f * turn, 0f);
            rigidBody.MoveRotation(rigidBody.rotation * turnRotation);

            if (engineAudio.clip == engineIdle)
            {
                engineAudio.clip = engineActive;
                engineAudio.Play();
            }
        }
        else
        {
            if (engineAudio.clip == engineActive)
            {
                engineAudio.clip = engineIdle;
                engineAudio.Play();
            }
            
        }
          
    }

    /// <summary>
	/// Handles cannon rotation. The top can only rotate 90 degrees left or right,
	/// and the barrel can only rotate 35 degrees up.
	/// </summary>
    private void rotateCannon()
    {
        // If inputting right, rotate cannon right
        if (Input.GetKey("right"))
        {
            // If cannon not max right, keep rotating
            if (cannonAngleH < maxCannonAngleH)
            {
                cannon.transform.localEulerAngles = new Vector3(0f, cannon.transform.localEulerAngles.y + (cannonSpeed * Time.deltaTime), 0f);
                cannonAngleH += cannonSpeed * Time.deltaTime;
            }
        }
        // If inputting left, rotate cannon left
        else if (Input.GetKey("left"))
        {
            // If cannon not max left, keep rotating
            if (cannonAngleH > -1 * maxCannonAngleH)
            {
                cannon.transform.localEulerAngles = new Vector3(0f, cannon.transform.localEulerAngles.y - (cannonSpeed * Time.deltaTime), 0f);
                cannonAngleH -= cannonSpeed * Time.deltaTime;
            }
        }

        // If inputting up, raise barrel of cannon
        if (Input.GetKey("up"))
        {
            // If barrel not at max height, keep raising
            if (cannonAngleV > maxCannonAngleV)
            {
                barrel.transform.localEulerAngles = new Vector3(barrel.transform.localEulerAngles.x - (cannonSpeed * Time.deltaTime), 0f, 0f);
                cannonAngleV -= cannonSpeed * Time.deltaTime;
            }
        }
        // If inputting down, lower barrel of cannon
        else if (Input.GetKey("down"))
        {
            // If barrel not at min height, keep lowering
            if (cannonAngleV < 0)
            {
                barrel.transform.localEulerAngles = new Vector3(barrel.transform.localEulerAngles.x + (cannonSpeed * Time.deltaTime), 0f, 0f);
                cannonAngleV += cannonSpeed * Time.deltaTime;
            }
        }
    }

    /// <summary>
	/// Handles wheel rotation. Wheels continously rotate 45 degrees in direction
	/// of turn and remain there until no longer turning. Wheels then return to normal.
	/// Front wheels rotate faster than back wheels.
	/// </summary>
    private void rotateWheels()
    {
        // Forward wheel rotation
        if (movementInput > 0f)
        {
            leftBackWheel.transform.Rotate(2f, 0f, 0f);
            leftFrontWheel.transform.Rotate(4f, 0f, 0f);
            rightBackWheel.transform.Rotate(2f, 0f, 0f);
            rightFrontWheel.transform.Rotate(4f, 0f, 0f);
        }
        // Backward wheel rotation
        else if (movementInput < 0f)
        {
            leftBackWheel.transform.Rotate(-2f, 0f, 0f);
            leftFrontWheel.transform.Rotate(-4f, 0f, 0f);
            rightBackWheel.transform.Rotate(-2f, 0f, 0f);
            rightFrontWheel.transform.Rotate(-4f, 0f, 0f);
        }

        // Turning right
        if (turnInput > 0f)
        {
            // If not at 45 degrees right, continue to turn
            if (tireAngle < maxTireAngle)
            {
                rightSteer.transform.localEulerAngles = new Vector3(0f, rightSteer.transform.localEulerAngles.y + (tireSpeed * Time.deltaTime), 0f);
                leftSteer.transform.localEulerAngles = new Vector3(0f, leftSteer.transform.localEulerAngles.y + (tireSpeed * Time.deltaTime), 0f);
                tireAngle += tireSpeed * Time.deltaTime;
            }
        }
        // Turning left
        else if (turnInput < 0f)
        {
            // If not at 45 degrees left, continue to turn
            if (tireAngle > -1 * maxTireAngle)
            {
                rightSteer.transform.localEulerAngles = new Vector3(0f, rightSteer.transform.localEulerAngles.y - (tireSpeed * Time.deltaTime), 0f);
                leftSteer.transform.localEulerAngles = new Vector3(0f, leftSteer.transform.localEulerAngles.y - (tireSpeed * Time.deltaTime), 0f);
                tireAngle -= tireSpeed * Time.deltaTime;
            }
        }
        // Not turning
        else
        {
            // Set back to 0 from right
            if (tireAngle >= 0)
            {
                rightSteer.transform.localEulerAngles = new Vector3(0f, rightSteer.transform.localEulerAngles.y - (tireSpeed * Time.deltaTime), 0f);
                leftSteer.transform.localEulerAngles = new Vector3(0f, leftSteer.transform.localEulerAngles.y - (tireSpeed * Time.deltaTime), 0f);
                tireAngle -= tireSpeed * Time.deltaTime;
            }

            // Set back to 0 from left
            if (tireAngle <= 0)
            {
                rightSteer.transform.localEulerAngles = new Vector3(0f, rightSteer.transform.localEulerAngles.y + (tireSpeed * Time.deltaTime), 0f);
                leftSteer.transform.localEulerAngles = new Vector3(0f, leftSteer.transform.localEulerAngles.y + (tireSpeed * Time.deltaTime), 0f);
                tireAngle += tireSpeed * Time.deltaTime;
            }
        }
    }

    /// <summary>
	/// Detects when the player has been hit by a shell. If so reduce health,
	/// display new health, and if health is 0 call on loseGame() from winLose()
	/// and detonate tank.
	/// </summary>
	/// <param name="collision"></param>
    private void OnCollisionEnter(Collision collision)
    {
        // If collision is a shell
        if (collision.gameObject.tag == "shell")
        {
            // Reduce and update health
            health--;
            healthText.GetComponent<Text>().text = health.ToString();

            // If out of health
            if (health == 0)
            {
                // Detach cameras and particle system, blow up tank
                tankExplosion.transform.parent = null;
                camera1.transform.parent = null;
                camera2.transform.parent = null;
                tankExplosion.Play();

                // Call on lose game and destroy detached objects
                canvas.GetComponent<winLose>().loseGame();
                Destroy(tankExplosion.gameObject, tankExplosion.main.duration);
                Destroy(gameObject);
            }
        }
    }

    /// <summary>
	/// Resets the game by loading the original game scene
	/// </summary>
    private void resetGame()
    {
        SceneManager.LoadScene("Main");
    }
}