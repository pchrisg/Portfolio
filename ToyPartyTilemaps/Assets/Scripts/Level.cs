using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class Level : MonoBehaviour
{
    public GameObject emptyPrefab;
    public GameObject highlightPrefab;
    public GameObject[] toyPrefabs;
    public GameObject topPrefab;
    public GameObject grid;
    public Tilemap playableArea;

    public List<GameObject> highlights = new List<GameObject>();
    
    private Vector3 spawnPos;
    private int spawnRow;
    private int spawnCol;

    private int numRows;
    private int numCols;
    private GameObject[,] toyList;

    // Start is called before the first frame update
    void Start()
    {
        grid = new GameObject
        {
            name = "grid"
        };

        SetGrid();
    }

    private void SetGrid()
    {
        Tilemap[] tilemaps = this.GetComponentsInChildren<Tilemap>();
        Tilemap special = null;
        foreach (Tilemap map in tilemaps)
        {
            if (map.name == "PlayableArea")
                playableArea = map;
            if (map.name == "Special")
                special = map;
        }

        if (playableArea == null || special == null)
        {
            Debug.Log("Error finding the tilemaps");
            return;
        }

        playableArea.CompressBounds();
        numRows = playableArea.cellBounds.size.x;
        numCols = playableArea.cellBounds.size.y;
        spawnRow = numRows - 1;

        toyList = new GameObject[numRows, numCols];
        int rowOffset = 0 - playableArea.cellBounds.xMin;
        int colOffset = 0 - playableArea.cellBounds.yMin;

        for (int row = 0; row < playableArea.cellBounds.xMax + rowOffset; row++)
        {
            for (int col = 0; col < playableArea.cellBounds.yMax + colOffset; col++)
            {
                Vector3Int localPlace = new Vector3Int(row - rowOffset, col - colOffset, (int)playableArea.transform.position.y);

                string validPlace = special.HasTile(localPlace) ? "special" :
                               playableArea.HasTile(localPlace) ? "playableArea" :
                                                                  "null";

                if (!string.Equals(validPlace, "null"))
                {
                    Vector3 place = playableArea.CellToWorld(localPlace);
                    if(row == spawnRow)
                    {
                        spawnCol = col;
                        spawnPos = place;
                    }

                    GameObject toy = null;
                    if (string.Equals(validPlace, "special"))
                    {
                        toy = Instantiate(topPrefab, place, Quaternion.identity);
                        Animator animator = toy.GetComponent<Animator>();
                        animator.SetBool("spin", false);
                    }
                    else if (string.Equals(validPlace, "playableArea"))
                    {
                        int randomToy = Random.Range(0, toyPrefabs.Length);
                        int checkCount = 0;
                        while (CreatesMatch(toyPrefabs[randomToy], row, col) && checkCount < 100)
                        {
                            randomToy = Random.Range(0, toyPrefabs.Length);
                            checkCount++;
                        }

                        toy = Instantiate(toyPrefabs[randomToy], place, Quaternion.identity);
                    }
                    
                    toy.name = "( " + row + ", " + col + " )";
                    toy.transform.parent = grid.transform;
                    Properties toy_P = toy.GetComponent<Properties>();
                    toy_P.SetRow(row);
                    toy_P.SetCol(col);
                    toy_P.SetTargetPos(toy.transform.position);
                    toyList[row, col] = toy;
                }
            }
        }
        Destroy(special.GetComponent<TilemapRenderer>());
    }

    private bool CreatesMatch(GameObject newToy, int row, int col)
    {
        if (row > 0)
        {
            //even column
            if (col % 2 == 0)
            {
                //up diagonal 2 back
                if (col > 1 &&
                    toyList[row, col - 1] != null && toyList[row - 1, col - 2] != null &&
                    newToy.CompareTag(toyList[row, col - 1].tag) && toyList[row, col - 1].CompareTag(toyList[row - 1, col - 2].tag))
                    return true;
            }
            //odd column
            else
            {
                //up diagonal 2 back
                if (col > 1 &&
                    toyList[row - 1, col - 1] != null && toyList[row - 1, col - 2] != null &&
                    newToy.CompareTag(toyList[row - 1, col - 1].tag) && toyList[row - 1, col - 1].CompareTag(toyList[row - 1, col - 2].tag))
                    return true;

                //down diagonal 1 back, 1 forward
                if (col > 0 && col < numCols - 1 &&
                    toyList[row, col - 1] != null && toyList[row - 1, col + 1] != null &&
                    newToy.CompareTag(toyList[row, col - 1].tag) && toyList[row, col - 1].CompareTag(toyList[row - 1, col + 1].tag))
                    return true;

                //down diagonal 2 forward
                if (col < numCols - 2 &&
                    toyList[row - 1, col + 1] != null && toyList[row - 1, col + 2] != null &&
                    newToy.CompareTag(toyList[row - 1, col + 1].tag) && toyList[row - 1, col + 1].CompareTag(toyList[row - 1, col + 2].tag))
                    return true;
            }
        }

        //2 down
        if (row > 1 &&
            toyList[row - 1, col] != null && toyList[row - 2, col] != null &&
            newToy.CompareTag(toyList[row - 1, col].tag) && toyList[row - 1, col].CompareTag(toyList[row - 2, col].tag))
            return true;

        return false;
    }

    public int RowSize()
    {
        return numRows;
    }

    public int ColSize()
    {
        return numCols;
    }

    public GameObject GetToy(int row, int col)
    {
        if (row < 0 || row > numRows - 1)
            return null;
        if (col < 0 || col > numCols - 1)
            return null;

        return toyList[row, col];
    }

    public void SetToy(int row, int col, GameObject toy)
    {
        if (row < 0 || row > numRows - 1)
            return;
        if (col < 0 || col > numCols - 1)
            return;

        toyList[row, col] = toy;
    }

    public bool EmptySpawn()
    {
        return toyList[spawnRow, spawnCol].CompareTag("Empty");
    }

    public GameObject GetSpawnToy()
    {
        return toyList[spawnRow, spawnCol];
    }

    public void SetSpawnToy(GameObject toy)
    {
        toyList[spawnRow, spawnCol] = toy;
    }

    public Vector3 SpawnOffset()
    {
        return spawnPos + new Vector3(0.0f, 1.0f, 0.0f);
    }
}