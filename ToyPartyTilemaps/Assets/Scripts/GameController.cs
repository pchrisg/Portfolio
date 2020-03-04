using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum GameState
{
    unlocked,
    locked
}

public class GameController : MonoBehaviour
{
    private const float MINDISTANCE = 0.1f;
    private const float PAUSETIME = 0.2f;
    private const float DELETETIME = 0.5f;

    private Level level;

    private Vector2 firstTouchPos;
    private Vector2 finalTouchPos;
    private GameObject touchedToy;
    private GameState gameState = GameState.unlocked;

    private bool hasEmptySpaces = false;
    private bool lookForMatches = false;
    private bool hasMatches = false;

    void Start()
    {
        level = FindObjectOfType<Level>();
    }

    void Update()
    {
        if (hasMatches)
            gameState = GameState.locked;

        if (!hasMatches && !hasEmptySpaces)
            gameState = GameState.unlocked;

        if (hasEmptySpaces)
        {
            lookForMatches = true;
            hasMatches = false;

            StartCoroutine(FillEmptySpaces());
        }

        if (!hasEmptySpaces && lookForMatches)
        {
            lookForMatches = false;
            gameState = GameState.locked;

            FindMatches();
            if (hasMatches)
                StartCoroutine(DeleteMatches());
        }

        if (gameState == GameState.unlocked && Input.GetMouseButtonDown(0))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            if (Physics.Raycast(ray, out RaycastHit hit))
            {
                firstTouchPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                touchedToy = hit.collider.gameObject;
            }
        }

        if (gameState == GameState.unlocked && Input.GetMouseButtonUp(0))
        {
            finalTouchPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);

            //if distance between touches is too small then ignore
            if (touchedToy != null && !touchedToy.CompareTag("Empty") && Vector2.Distance(firstTouchPos, finalTouchPos) > MINDISTANCE)
            {
                gameState = GameState.locked;

                GameObject otherToy = FindSwapToy(touchedToy);
                if (otherToy != null && !otherToy.CompareTag("Empty"))
                {
                    SwapPos(touchedToy, otherToy);
                    FindMatches();
                    if (!hasMatches)
                        StartCoroutine(SwapBack(touchedToy, otherToy));
                    else
                        StartCoroutine(DeleteMatches());
                }

                touchedToy = null;
                gameState = GameState.unlocked;
            }
        }
    }

    private GameObject FindSwapToy(GameObject touchedToy)
    {
        Properties tToy_P = touchedToy.GetComponent<Properties>();
        float swipeAngle = Mathf.Atan2(finalTouchPos.y - firstTouchPos.y, finalTouchPos.x - firstTouchPos.x) * 180 / Mathf.PI;

        //if even column, the side values in the row go down
        //if odd column,  the side values in the row go up
        bool sameRowGoesDown = tToy_P.GetCol() % 2 == 0 ? true : false;

        //if up swipe
        if (swipeAngle >= 60 && swipeAngle < 120)
            return level.GetToy(tToy_P.GetRow() + 1, tToy_P.GetCol());

        //if up-right swipe
        if (swipeAngle >= 0 && swipeAngle < 60)
            return sameRowGoesDown ?
                       level.GetToy(tToy_P.GetRow() + 1, tToy_P.GetCol() + 1) : //if even column
                       level.GetToy(tToy_P.GetRow(), tToy_P.GetCol() + 1);      //if odd column

        //if down-right swipe
        if (swipeAngle >= -60 && swipeAngle < 0)
            return sameRowGoesDown ?
                       level.GetToy(tToy_P.GetRow(), tToy_P.GetCol() + 1) :     //if even column
                       level.GetToy(tToy_P.GetRow() - 1, tToy_P.GetCol() + 1);  //if odd column

        //if down swipe and value within range
        if (swipeAngle >= -120 && swipeAngle < -60)
            return level.GetToy(tToy_P.GetRow() - 1, tToy_P.GetCol());

        //if down-left swipe
        if (swipeAngle >= -180 && swipeAngle < -120)
            return sameRowGoesDown ?
                       level.GetToy(tToy_P.GetRow(), tToy_P.GetCol() - 1) :     //if even column
                       level.GetToy(tToy_P.GetRow() - 1, tToy_P.GetCol() - 1);  //if odd column

        //if up-left swipe
        if (swipeAngle >= 120 && swipeAngle < 180)
            return sameRowGoesDown ?
                       level.GetToy(tToy_P.GetRow() + 1, tToy_P.GetCol() - 1) : //if even column
                       level.GetToy(tToy_P.GetRow(), tToy_P.GetCol() - 1);      //if odd column
        return null;
    }

    private void SwapPos(GameObject toy1, GameObject toy2)
    {

        Properties toy1_P = toy1.GetComponent<Properties>();
        Properties toy2_P = toy2.GetComponent<Properties>();

        int tempRow = toy1_P.GetRow();
        int tempCol = toy1_P.GetCol();
        Vector3 tempTargetPos = toy1_P.GetTargetPos();
        string tempName = toy1_P.name;

        level.SetToy(toy2_P.GetRow(), toy2_P.GetCol(), toy1);
        toy1.name = toy2.name;

        toy1_P.SetRow(toy2_P.GetRow());
        toy1_P.SetCol(toy2_P.GetCol());
        toy1_P.SetTargetPos(toy2_P.GetTargetPos());

        level.SetToy(tempRow, tempCol, toy2);
        toy2.name = tempName;

        toy2_P.SetRow(tempRow);
        toy2_P.SetCol(tempCol);
        toy2_P.SetTargetPos(tempTargetPos);
    }

    private void FindMatches()
    {
        for (int row = 0; row < level.RowSize(); ++row)
        {
            for (int col = 0; col < level.ColSize(); ++col)
            {
                GameObject toy = level.GetToy(row, col);

                if (toy == null)
                    continue;

                if (toy.CompareTag("Top") || toy.CompareTag("Empty"))
                    continue;

                Properties toy_P = toy.GetComponent<Properties>();
                GameObject[] surroundingToys = new GameObject[6];
                GetSurroundingToys(toy_P ,ref surroundingToys);

                for (int i = 0; i < 3; ++i)
                {
                    if (surroundingToys[i] != null && surroundingToys[i].CompareTag(toy.tag))
                    {
                        if (surroundingToys[i + 3] != null && surroundingToys[i + 3].CompareTag(toy.tag))
                        {
                            hasMatches = true;

                            GameObject highlight;
                            toy_P.Match();
                            highlight = Instantiate(level.highlightPrefab, toy_P.GetTargetPos(), Quaternion.identity);
                            level.highlights.Add(highlight);

                            Properties sToy1_P = surroundingToys[i].GetComponent<Properties>();
                            sToy1_P.Match();
                            highlight = Instantiate(level.highlightPrefab, sToy1_P.GetTargetPos(), Quaternion.identity);
                            level.highlights.Add(highlight);

                            Properties sToy2_P = surroundingToys[i + 3].GetComponent<Properties>();
                            sToy2_P.Match();
                            highlight = Instantiate(level.highlightPrefab, sToy2_P.GetTargetPos(), Quaternion.identity);
                            level.highlights.Add(highlight);
                        }
                    }
                }
            }
        }
    }

    private IEnumerator SwapBack(GameObject touchedToy, GameObject otherToy)
    {
        yield return new WaitForSeconds(PAUSETIME);

        SwapPos(touchedToy, otherToy);
    }

    private void LookAroundForTops(Properties toy_P, ref List<GameObject> topsToSpin)
    {
        //if even column, the side values in the row go down
        //if odd column,  the side values in the row go up
        bool rowGoesDown = toy_P.GetCol() % 2 == 0 ? true : false;

        GameObject[] surroundingToys = new GameObject[6];
        GetSurroundingToys(toy_P, ref surroundingToys);

        foreach (GameObject surroundingToy in surroundingToys)
        {
            if (surroundingToy != null && surroundingToy.CompareTag("Top") && !topsToSpin.Contains(surroundingToy))
                topsToSpin.Add(surroundingToy);
        }
    }

    private void GetSurroundingToys(Properties toy_P, ref GameObject[] surroundingToys)
    {
        //if even column, the side values in the row go down
        //if odd column,  the side values in the row go up
        bool rowGoesDown = toy_P.GetCol() % 2 == 0 ? true : false;

        //up
        surroundingToys[0] = level.GetToy(toy_P.GetRow() + 1, toy_P.GetCol());

        //up-right
        surroundingToys[1] = rowGoesDown ?
                                level.GetToy(toy_P.GetRow() + 1, toy_P.GetCol() + 1) :
                                level.GetToy(toy_P.GetRow(), toy_P.GetCol() + 1);
        //down-right
        surroundingToys[2] = rowGoesDown ?
                                level.GetToy(toy_P.GetRow(), toy_P.GetCol() + 1) :
                                level.GetToy(toy_P.GetRow() - 1, toy_P.GetCol() + 1);
        //down
        surroundingToys[3] = level.GetToy(toy_P.GetRow() - 1, toy_P.GetCol());

        //down-left
        surroundingToys[4] = rowGoesDown ?
                                level.GetToy(toy_P.GetRow(), toy_P.GetCol() - 1) :
                                level.GetToy(toy_P.GetRow() - 1, toy_P.GetCol() - 1);
        //up-left
        surroundingToys[5] = rowGoesDown ?
                                level.GetToy(toy_P.GetRow() + 1, toy_P.GetCol() - 1) :
                                level.GetToy(toy_P.GetRow(), toy_P.GetCol() - 1);
    }

    private IEnumerator DeleteMatches()
    {
        yield return new WaitForSeconds(DELETETIME);

        List<GameObject> topsToSpin = new List<GameObject>();
        for (int row = 0; row < level.RowSize(); row++)
        {
            for (int col = 0; col < level.ColSize(); col++)
            {
                if (level.GetToy(row, col) == null)
                    continue;

                GameObject toy = level.GetToy(row, col);
                Properties toy_P = toy.GetComponent<Properties>();
                if (toy_P.IsMatched())
                {
                    hasEmptySpaces = true;

                    LookAroundForTops(toy_P, ref topsToSpin);

                    GameObject empty;
                    empty = Instantiate(level.emptyPrefab, toy.transform.position, Quaternion.identity);
                    empty.name = toy.name;
                    empty.transform.parent = level.grid.transform;

                    Properties empty_P = empty.GetComponent<Properties>();
                    empty_P.SetRow(toy_P.GetRow());
                    empty_P.SetCol(toy_P.GetCol());
                    empty_P.SetTargetPos(toy_P.GetTargetPos());

                    Destroy(toy);
                    level.SetToy(row, col, empty);
                }
            }
        }

        if (topsToSpin.Count > 0)
        {
            foreach (GameObject top in topsToSpin)
            {
                Animator animator = top.GetComponent<Animator>();
                if (animator.GetBool("spin"))
                {
                    Properties top_P = top.GetComponent<Properties>();

                    GameObject empty;
                    empty = Instantiate(level.emptyPrefab, top.transform.position, Quaternion.identity);
                    empty.name = top.name;
                    empty.transform.parent = level.grid.transform;

                    Properties empty_P = empty.GetComponent<Properties>();
                    empty_P.SetRow(top_P.GetRow());
                    empty_P.SetCol(top_P.GetCol());
                    empty_P.SetTargetPos(top_P.GetTargetPos());

                    level.SetToy(top_P.GetRow(), top_P.GetCol(), empty);
                    Destroy(top);
                }
                else
                    animator.SetBool("spin", true);
            }
        }

        foreach (GameObject highlight in level.highlights)
        {
            Destroy(highlight);
        }
        level.highlights.Clear();
    }

    private IEnumerator FillEmptySpaces()
    {
        yield return new WaitForSeconds(PAUSETIME);

        hasEmptySpaces = false;

        Gravity();
        Repopulate();
    }

    private void Gravity()
    {
        for (int row = 0; row < level.RowSize() - 1; ++row)
        {
            for (int col = 0; col < level.ColSize(); ++col)
            {
                GameObject toy = level.GetToy(row, col);
                if (toy != null && toy.CompareTag("Empty"))
                {
                    if (level.GetToy(row + 1, col) != null && !level.GetToy(row + 1, col).CompareTag("Empty"))
                    {
                        SwapPos(toy, level.GetToy(row + 1, col));
                        continue;
                    }

                    bool rowGoesDown = col % 2 == 0 ? true : false;
                    if (rowGoesDown)
                    {
                        if (level.GetToy(row + 1, col - 1) != null && !level.GetToy(row + 1, col - 1).CompareTag("Empty"))
                        {
                            SwapPos(toy, level.GetToy(row + 1, col - 1));
                            continue;
                        }
                        if (level.GetToy(row + 1, col + 1) != null && !level.GetToy(row + 1, col + 1).CompareTag("Empty"))
                        {
                            SwapPos(toy, level.GetToy(row + 1, col + 1));
                            continue;
                        }
                    }
                    else
                    {
                        if (level.GetToy(row, col + 1) != null && !level.GetToy(row, col + 1).CompareTag("Empty"))
                        {
                            SwapPos(toy, level.GetToy(row, col + 1));
                            continue;
                        }
                        if (level.GetToy(row, col - 1) != null && !level.GetToy(row, col - 1).CompareTag("Empty"))
                        {
                            SwapPos(toy, level.GetToy(row, col - 1));
                            col -= 2;
                            continue;
                        }
                    }
                }
            }
        }
    }

    private void Repopulate()
    {
        if (!level.EmptySpawn())
            return;

        hasEmptySpaces = true;

        GameObject empty = level.GetSpawnToy();
        Properties empty_P = empty.GetComponent<Properties>();

        GameObject newToy;
        int randomToy = Random.Range(0, level.toyPrefabs.Length);
        newToy = Instantiate(level.toyPrefabs[randomToy], level.SpawnOffset(), Quaternion.identity);
        newToy.transform.parent = level.grid.transform;
        newToy.name = empty.name;

        Properties newToy_P = newToy.GetComponent<Properties>();
        newToy_P.SetRow(empty_P.GetRow());
        newToy_P.SetCol(empty_P.GetCol());
        newToy_P.SetTargetPos(empty_P.GetTargetPos());

        level.SetSpawnToy(newToy);

        Destroy(empty);
    }
}