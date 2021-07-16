using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// to check neighbours - the physics is used if overlapping circle collide with each other
[RequireComponent(typeof(Collider2D))]
[RequireComponent(typeof(Rigidbody2D))]


public class FlockAgent : MonoBehaviour
{
    Collider2D agentCollider;
    Rigidbody2D agentRigidBody;
    public Collider2D AgentCollider { get { return agentCollider; } } // public accesor
    public Rigidbody2D AgentRigidBody { get { return agentRigidBody; } }

    // Start is called before the first frame update
    void Start()
    {
        agentCollider = GetComponent<Collider2D>();
        agentCollider.isTrigger = true;
        agentRigidBody = GetComponent<Rigidbody2D>();
        agentRigidBody.bodyType = RigidbodyType2D.Static;
        Physics2D.gravity = Vector2.zero;
    }

    public void Move(Vector2 velocity)
    {
        transform.up = velocity;
        transform.position += (Vector3)velocity * Time.deltaTime; // Constant movement regardless of framerate
    }
}
