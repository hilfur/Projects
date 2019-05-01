using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : MonoBehaviour
{

    public float moveSpeed;
    public float jumpforce;
    public float lastJump;
    public CapsuleCollider col;
    public LayerMask groundLayers;
    public Rigidbody rb;
    public float distToGround;
    public bool isgrounded;
    public float timeLeft;
    public Vector3 startPosition;

    void Awake()
    {
        startPosition = transform.position; 

    }
    void Start()
    {
        moveSpeed = 5f;
        jumpforce = 7f;
        timeLeft = 0f; 
        rb = GetComponent<Rigidbody>();
        col = GetComponent<CapsuleCollider>();
        distToGround = col.bounds.extents.y;




    }

    // Update is called once per frame
    void Update()
    {
        timeLeft -= Time.deltaTime;
        print(timeLeft);
    }
    // Kilde på movement: https://www.youtube.com/watch?v=blO039OzUZc
    void FixedUpdate()
    {
        
        float moveHorizontal = Input.GetAxis("Horizontal");
        float moveVertical = Input.GetAxis("Vertical");
        transform.Translate(moveSpeed * moveHorizontal * Time.deltaTime, 0f, moveSpeed * moveVertical * Time.deltaTime);

        if (IsGrounded() && Input.GetKeyDown(KeyCode.Space))
        {
            rb.AddForce(Vector3.up * jumpforce, ForceMode.Impulse);
        }
        if (IsGrounded() && Input.GetAxis("Mouse ScrollWheel") < 0f && timeLeft > 1.5f)
        {
            timeLeft = 3f; 
            rb.AddForce(Vector3.up * jumpforce, ForceMode.Impulse);
            moveSpeed += 1f; 
        }
        else if(IsGrounded() && Input.GetAxis("Mouse ScrollWheel") < 0f && timeLeft < 1.5f)
        {
            timeLeft = 3f;
            rb.AddForce(Vector3.up * jumpforce, ForceMode.Impulse);
            moveSpeed = 5f;
        }
        if(timeLeft < 1.5 && IsGrounded())
        {
            moveSpeed = 5f; 
        }
        if (Input.GetKey("r"))
        {
            transform.position = startPosition;
        }

    }

    private bool IsGrounded()
    { 
        return Physics.Raycast(transform.position, -Vector3.up, distToGround + 0.1f);
    }
    private void OnTriggerEnter(Collider col)
    {
        if(col.gameObject.tag == "lava")
        {
            transform.position = startPosition;

        }
        else if(col.gameObject.tag == "portal")
        {
            SceneManager.LoadScene(2);
           
        }
        else if(col.gameObject.tag == "level1")
        {
            SceneManager.LoadScene(1);
        }
    }

}
