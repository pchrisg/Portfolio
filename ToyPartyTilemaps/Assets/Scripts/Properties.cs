using UnityEngine;

public class Properties : MonoBehaviour
{
    private const float SWAPSPEED = 0.2f;
    private const float SNAPDISTANCE = 0.5f;

    private int row;
    private int col;
    private Vector3 targetPos;
    private bool isMatched = false;

    public void SetRow(int row)
    {
        this.row = row;
    }

    public int GetRow()
    {
        return this.row;
    }

    public void SetCol(int col)
    {
        this.col = col;
    }

    public int GetCol()
    {
        return this.col;
    }

    public void SetTargetPos(Vector3 pos)
    {
        this.targetPos = pos;
    }

    public Vector3 GetTargetPos()
    {
        return this.targetPos;
    }

    public void Match()
    {
        this.isMatched = true;
    }

    public void UnMatch()
    {
        this.isMatched = false;
    }

    public bool IsMatched()
    {
        return this.isMatched;
    }

    void Update()
    {
        // If current and target positions don't match, move to target
        if(transform.position != targetPos)
            Swap();
    }

    private void Swap()
    {
        if (Vector2.Distance(transform.position, targetPos) > SNAPDISTANCE)
            transform.position = Vector3.Lerp(transform.position, targetPos, SWAPSPEED);
        else
            transform.position = targetPos;
    }
}