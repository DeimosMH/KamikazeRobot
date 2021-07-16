using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Flock : MonoBehaviour
{
    public FlockAgent agentPrefab;

    // Empty list, if flock populate itself its gon to iterate through them
    List<FlockAgent> agents = new List<FlockAgent>();
    public FlockBehaviour behaviour;

    public Transform enemyLocalisation;

    public Vector2 colliderSize = Vector2.one;

    [Range(1, 2000)]                  // populating flock 
    public int startingCount = 250;
    const float AgentDensity = 0.1f; // based on number of agents in circle - populating flock randomly within circle

    [Range(1f, 100f)]
    public float driveFactor = 10f;   // making agents move fater
    [Range(1f, 100f)]
    public float maxSpeed = 5f;
    [Range(1f, 10f)]
    public float neighbourRadius = 1.5f;
    [Range(0f, 2f)]
    public float avoidanceRadiusMultiplier = 0.5f;

    // Arrangement of agents 
    public bool arrayFlock = true;
    [Range(1, 100)]
    public int arrayArmy = 10;
    
    public float beginAtX = 0f;

    public float beginAtY = 0f;


    // Edges
    public bool edgesTeleportingObjectsScreenEdges = true;
    // 
    [Range(10, 100)]
    public int width_EdgesTeleportingObjects = 100;
    [Range(10, 100)]
    public int height_EdgesTeleportingObjects = 100;

    // Start
    public bool enemyDetected = false;

    ////Assign this Camera in the Inspector
    //public Camera m_OrthographicCamera;
    ////These are the positions and dimensions of the Camera view in the Game view
    //float m_ViewPositionX, m_ViewPositionY, m_Viewwidth_EdgesTeleportingObjects, m_Viewheight_EdgesTeleportingObjects;

    float squareMaxSpeed;
    float squareNeighbourRadius;
    float squareAvoidanceRadius;
    public float SquareAvoidanceRadius { get { return squareAvoidanceRadius;  } }

    // Start is called before the first frame update
    void Start()
    {

        //This sets the Camera view rectangle to be in the bottom corner of the screen
        //m_ViewPositionX = 0;
        //m_ViewPositionY = 0;
        //m_Viewwidth_EdgesTeleportingObjects = width_EdgesTeleportingObjects;
        //m_Viewheight_EdgesTeleportingObjects = height_EdgesTeleportingObjects;
        //m_OrthographicCamera.enabled = true;
        //m_OrthographicCamera.orthographic = true;
        //m_OrthographicCamera.orthographicSize = width_EdgesTeleportingObjects;
        //m_OrthographicCamera.rect = new Rect(m_ViewPositionX, m_ViewPositionY, m_Viewwidth_EdgesTeleportingObjects, m_Viewheight_EdgesTeleportingObjects);


        squareMaxSpeed = maxSpeed * maxSpeed;
        squareNeighbourRadius = neighbourRadius * neighbourRadius;
        squareAvoidanceRadius = squareNeighbourRadius * avoidanceRadiusMultiplier * avoidanceRadiusMultiplier;

        for (int i = 0; i < startingCount; i++)
        {
            FlockAgent newAgent;
            if (arrayFlock == true)
            { 
                newAgent = Instantiate(
                agentPrefab,
                transform
                );
            }
            else
            {
                newAgent = Instantiate(
                agentPrefab,
                Random.insideUnitCircle * startingCount * AgentDensity,     // instatiate inside random point within circle
                Quaternion.Euler(Vector3.forward * Random.Range(0f, 360f)), // random vect 3 rotation between 0 and 360 on z axes, converted to quartenions
                transform
                );
            }

            newAgent.name = "Agent" + i;

            agents.Add(newAgent); // adding new agent to the list
        }


        if (arrayFlock == true)
        {
            int ii = 0;
            int jj = 0;
            foreach (FlockAgent agent in agents)
            {
                if (ii % arrayArmy == 0)
                {
                    jj = jj + 1;
                    ii = 0;
                }

                agent.transform.position = new Vector3(ii + beginAtX, jj + beginAtY, 1);
                ii++;
            }
        }

    }



    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey("a"))
        {
            enemyDetected = true;
        }


        if (enemyDetected == true)
        {

            foreach (FlockAgent agent in agents)  // iterating through every agent in foreach loop in list
            {
                if (agent.AgentRigidBody.bodyType == RigidbodyType2D.Static)
                {
                    agent.AgentRigidBody.bodyType = RigidbodyType2D.Dynamic;
                    Debug.Log("Dynamic change");
                }
            }

            if (edgesTeleportingObjectsScreenEdges == true)
            {
                foreach (FlockAgent agent in agents)  // iterating through every agent in foreach loop in list
                {
                    List<Transform> context = GetNearbyObjects(agent);

                    // test FOR DEMO ONLY
                    //agent.GetComponentInChildren<SpriteRenderer>().color = Color.Lerp(Color.white, Color.red, context.Count / 6f); // if 6neighbours turn red

                    Vector2 move = behaviour.CalculateMove(agent, context, this); // this - flock
                    move *= driveFactor;

                    if (move.sqrMagnitude > squareMaxSpeed)
                    {
                        move = move.normalized * maxSpeed;
                    }
                    agent.Move(move);

                    if (agent.transform.position.x >= width_EdgesTeleportingObjects)
                    {
                        agent.transform.position = new Vector2(-width_EdgesTeleportingObjects + 1, agent.transform.position.y);
                    }

                    if (agent.transform.position.x <= -width_EdgesTeleportingObjects)
                    {
                        agent.transform.position = new Vector2(width_EdgesTeleportingObjects - 1, agent.transform.position.y);

                    }

                    if (agent.transform.position.y >= height_EdgesTeleportingObjects)
                    {
                        agent.transform.position = new Vector2(agent.transform.position.x, -height_EdgesTeleportingObjects + 1);
                    }

                    if (agent.transform.position.y <= -height_EdgesTeleportingObjects)
                    {
                        agent.transform.position = new Vector2(agent.transform.position.x, height_EdgesTeleportingObjects + 1);
                    }

                }
            }
            else
            {
                foreach (FlockAgent agent in agents)  // iterating through every agent in foreach loop in list
                {
                    List<Transform> context = GetNearbyObjects(agent);

                    // test FOR DEMO ONLY
                    //agent.GetComponentInChildren<SpriteRenderer>().color = Color.Lerp(Color.white, Color.red, context.Count / 6f); // if 6neighbours turn red

                    Vector2 move = behaviour.CalculateMove(agent, context, this); // this - flock
                    move *= driveFactor;
                    if (move.sqrMagnitude > squareMaxSpeed)
                    {
                        move = move.normalized * maxSpeed;
                    }
                    agent.Move(move);
                }
            }
        }
    }

    List<Transform> GetNearbyObjects(FlockAgent agent)
    {
        List<Transform> context = new List<Transform>();
        Collider2D[] contextColliders = Physics2D.OverlapCircleAll(agent.transform.position, neighbourRadius);

        foreach (Collider2D c in contextColliders)
        {
            if (c != agent.AgentCollider)
            {
                context.Add(c.transform);
            }
        }
        return context;
    }


    public void OnTriggerEnter2D(Collider2D collision)
    {
        Debug.Log("YAY!");
    }
}
