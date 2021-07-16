using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Flock/Behaviour/Composite")] //Steered Cohesion
public class CompositeBehaviour : FilteredFlockBehaviour // We are inheriting from flock behaviour
{
    public FlockBehaviour[] behaviours;
    public float[] weights; // weights of behaviours

    public override Vector2 CalculateMove(FlockAgent agent, List<Transform> context, Flock flock)
    {
        // handling mismatch
        if (weights.Length != behaviours.Length)
        {
            Debug.LogError("Data mismatch in " + name, this);
            return Vector2.zero; // dont move at all (whatever this agent is)
        }

        // set up move
        Vector2 move = Vector2.zero;

        // iterate through behaviours
        for (int i = 0; i < behaviours.Length; i++)
        {
            // to make sure we are using the same index on the behaviour and weights - that why for loop not foreach
            Vector2 partialMove = behaviours[i].CalculateMove(agent, context, flock) * weights[i];

            if (partialMove != Vector2.zero)
            {
                if (partialMove.sqrMagnitude > weights[i] * weights[i])
                {
                    partialMove.Normalize();
                    partialMove *= weights[i];
                }
                
                move += partialMove; // if magnitude lower than weight, then pass it as normal
            }

            // add all neighbours
            // if we dont have filter than we'll just use context as it is; If we have filter then we apply that
            List<Transform> filteredContext = (filter == null) ? context : filter.Filter(agent, context);
            foreach (Transform item in filteredContext)
            {
                if ((Vector2.SqrMagnitude(item.position - agent.transform.position)) < flock.SquareAvoidanceRadius)
                {
                    partialMove += (Vector2)(agent.transform.position - item.position);
                }
            }
        }

        return move;
    }
}