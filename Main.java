import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class BDD {
    private int numVars, numNodes;
    private BNode[] bdd;
    private Integer data[][];
    private TruthTable truthTable;

    public BDD(String path) throws IOException {
        setNumVars(path);
        setNumNodes(path);
        // Populates 2d-int array with input data used to build bdd
        extractDataFromFile(path);
        this.bdd = new BNode[numNodes];
        buildBDD();
        this.truthTable = new TruthTable(numVars);
    }

    private void extractDataFromFile(String path) throws IOException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Do not want the first two lines of data
        line = scanner.nextLine();
        line = scanner.nextLine();

        // Populate 2d array of ints
        this.data = new Integer[numNodes][4];
        for(int i = 0; i < numNodes; i++) {
            for(int j = 0; j < 4; j++) {
                this.data[i][j] = scanner.nextInt();
            }
        }
    }

    private void buildBDD() {
        for(int i = 0; i < numNodes; i++) {
            // Data format: data[i][0] = id, data[i][3] = decisionId
            this.bdd[i] = new BNode(this.data[i][0], this.data[i][3]);
        }
        int leftChildIndex = 0;
        int rightChildIndex = 0;
        // Populate left and right children nodes
        for(int i = 0; i < numNodes; i++) {
            leftChildIndex = data[i][1];
            rightChildIndex = data[i][2];
            // Node with no children has -1 value for data[i][1] and data[i][2]
            // No node will have 1 child. Node either has 2 children or 0.
            if (leftChildIndex > -1) {
                this.bdd[i].setLeftChild(leftChildIndex);
                this.bdd[i].setRightChild(rightChildIndex);
            }
        }
    }

    private int evaluateSingleBddInstance(BNode node, Integer vars[]) {
        // Base case, 0 children (There will never be only 1 child. Either 0 or 2.)
        if (node.leftChild == -1) {
            return node.decisionId;
        }

        // Array index starts at 0, node_id's start at 1. -1 aligns the data
        int index = node.decisionId - 1;
        // Branch towards left child if variable = 0, else branch towards right child (variable = 1)
        if(vars[index] == 0) {
            return evaluateSingleBddInstance(bdd[node.leftChild-1], vars);
        } else {
            return evaluateSingleBddInstance(bdd[node.rightChild-1], vars);
        }
    }

    public void evaluateAllBddInstances() {
        List<Integer[]> inputs = truthTable.getTruthTableInputs();
        for (Integer[] input : inputs) {
            truthTable.printTruthTableInput(input);
            System.out.println("  " + evaluateSingleBddInstance(bdd[0], input));
        }
    }

    public void printBDD() {
        for(BNode node : this.bdd) {
            System.out.println(node.id + " " + node.leftChild
                    + " " + node.rightChild + " " + node.decisionId);
        }
    }

    private void setNumVars(String path) throws FileNotFoundException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Extract int from first line of input (contains number of vars)
        line = scanner.nextLine();
        // Removes all chars from string using regex
        String numVarsStr = line.replaceAll("[^0-9]", "");
        this.numVars = Integer.parseInt(numVarsStr);
    }

    private void setNumNodes(String path) throws FileNotFoundException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Extract int from second line of input (contains number of nodes)
        line = scanner.nextLine();
        line = scanner.nextLine();
        // Removes all chars from string using regex
        String numNodesStr = line.replaceAll("[^0-9]", "");
        this.numNodes = Integer.parseInt(numNodesStr);
    }

    private class BNode {
        private int id, decisionId;
        private int leftChild, rightChild;

        public BNode(int id, int decisionId) {
            this.id = id;
            this.decisionId = decisionId;
            this.leftChild = -1;
            this.rightChild = -1;
        }

        public void setLeftChild(int leftChild) {
            this.leftChild = leftChild;
        }

        public void setRightChild(int rightChild) {
            this.rightChild = rightChild;
        }
    }
}

class Xbar {
    private int numVars, numRows, numCols;
    private Integer data[][];
    private TruthTable truthTable;
    int output;

    public Xbar(String path) throws IOException {
        this.output = 0;
        setNumVars(path);
        setNumRows(path);
        setNumCols(path);
        extractDataFromFile(path);
        truthTable = new TruthTable(numVars);
    }

    private void extractDataFromFile(String path) throws IOException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Do not want the first three lines of data
        line = scanner.nextLine();
        line = scanner.nextLine();
        line = scanner.nextLine();

        // Populate 2d array of ints
        this.data = new Integer[numRows][numCols];
        for(int i = 0; i < numRows; i++) {
            for(int j = 0; j < numCols; j++) {
                this.data[i][j] = scanner.nextInt();
            }
        }
    }

    public void passInstance(Integer instance[]) {
        Integer xbar[][] = createXbar(instance);
        evaluateSingleXbarInstance(xbar);
    }

    // Search algorithm
    public void evaluateSingleXbarInstance(Integer[][] instance) {
        searchDownward(instance, 0, numCols-1);
    }

    public void evaluateAllXbarInstances() {
        List<Integer[]> inputs = this.truthTable.getTruthTableInputs();
        for (Integer[] input : inputs) {
            this.truthTable.printTruthTableInput(input);
            passInstance(input);
            System.out.println(" " + this.output);
            setOutput(0);
        }
    }

    private void searchDownward(Integer[][] xbar, Integer rowIndex, Integer columnIndex) {
        for(int i = rowIndex; i < numRows; i++) {
            if ((i == numRows - 1)){
                if(xbar[i][columnIndex] == 1) {
                    setOutput(1);
                    return;
                }
            }
            if (xbar[i][columnIndex] == 1) {
                searchleft(xbar, i, columnIndex-1);
            }
        }
    }

    private void searchleft(Integer[][] xbar, Integer rowIndex, Integer columnIndex) {
        for(int j = columnIndex; j >= 0; j--) {
            if (xbar[rowIndex][j] == 1) {
                searchDownward(xbar, rowIndex+1, j);
            }
        }
    }

    public Integer[][] createXbar(Integer instance[]) {
        Integer xbar[][] = new Integer[numRows][numCols];
        int var;
        int varIndex;
        boolean isNegative;

        // Loop through data containing variables and on/off remsistors
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < data[i].length; j++) {
                // Case where data at current loc is off
                if (data[i][j] == 0) {
                    xbar[i][j] = 0;
                }// Case where data at current loc is on
                else if (data[i][j] == 99) {
                    xbar[i][j] = 1;
                }// Case where data at current loc is a variable
                else {
                    var = data[i][j];
                    // Check if variable is uncomplemented or complemented
                    if (var < 0) { // complemented var
                        isNegative = true;
                    } else { // uncomplemented var
                        isNegative = false;
                    }
                    varIndex = Math.abs(var) - 1; // Index of vars in instance are offset by 1
                    if(instance[varIndex] == 1) { // Instance var is true
                        if (isNegative) {
                            xbar[i][j] = 0; // Data var is complemented, hence turn off
                        } else {
                            xbar[i][j] = 1; // Data var is uncomplemented, hence turn on
                        }
                    } else { // Instance var is false
                        if (isNegative) {
                            xbar[i][j] = 1; // Data var is complemented, hence turn on
                        } else {
                            xbar[i][j] = 0; // Data var is uncomplemented, hence turn off
                        }
                    }
                }
            }
        }
        return xbar;
    }

    public void printXbar() {
        for(Integer[] arr : this.data) {
            for(Integer value : arr) {
                System.out.print(value + " ");
            }
            System.out.println();
        }
    }

    public void writeXbarToFile(String path) throws IOException {
        FileWriter writer = new FileWriter(path);
        for(Integer[] arr : this.data) {
            for(Integer value : arr) {
                writer.write(value + " ");
            }
            writer.write("\n");
        }
        writer.close();
    }

    private void setNumVars(String path) throws FileNotFoundException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Extract int from first line of input (contains number of vars)
        line = scanner.nextLine();
        // Removes all chars from string using regex
        String numVarsStr = line.replaceAll("[^0-9]", "");
        this.numVars = Integer.parseInt(numVarsStr);
    }

    private void setNumRows(String path) throws FileNotFoundException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Extract int from second line of input (contains number of rows)
        line = scanner.nextLine();
        line = scanner.nextLine();
        // Removes all chars from string using regex
        String numRowsStr = line.replaceAll("[^0-9]", "");
        this.numRows = Integer.parseInt(numRowsStr);
    }

    private void setNumCols(String path) throws FileNotFoundException {
        File file = new File(path);
        Scanner scanner = new Scanner(file);
        String line = null;
        // Extract int from third line of input (contains number of nodes)
        line = scanner.nextLine();
        line = scanner.nextLine();
        line = scanner.nextLine();
        // Removes all chars from string using regex
        String numColsStr = line.replaceAll("[^0-9]", "");
        this.numCols = Integer.parseInt(numColsStr);
    }

    public int getNumVars() {
        return numVars;
    }

    public int getNumRows() {
        return numRows;
    }

    public int getNumCols() {
        return numCols;
    }

    public int getOutput() {
        return output;
    }

    private void setOutput(int output) {
        this.output = output;
    }
}

class TruthTable {
    private List<Integer[]> truthTableInputs;

    public TruthTable(int numVars) {
        setTruthTableInputs(numVars);
    }

    // Uses generateInputInstances to populate ArrayList
    private void setTruthTableInputs(int numVars) {
        truthTableInputs = new ArrayList<>();
        Integer tempArr[] = new Integer[numVars];
        generateInputInstances(tempArr, numVars, 0);
    }

    private void generateInputInstances(Integer tempArr[], int numVars, int index) {
        // Base case
        if(index == numVars) {
            Integer tempArrClone[] = tempArr.clone();
            truthTableInputs.add(tempArrClone);
            return;
        }
        tempArr[index] = 0;
        generateInputInstances(tempArr, numVars, index+1);
        tempArr[index] = 1;
        generateInputInstances(tempArr, numVars, index+1);
    }

    public List<Integer[]> getTruthTableInputs() {
        return truthTableInputs;
    }

    public void printTruthTableInput(Integer[] instance) {
        for(Integer value : instance) {
            System.out.print(value + " ");
        }
    }
}

public class Main {
    public static void main(String[] args) throws IOException {
        String path = "bdds/var5.bdd";
        BDD bdd = new BDD(path); // Task 1.1
        bdd.printBDD(); // Task 1.2
        bdd.evaluateAllBddInstances(); // Task 2.3 and Task 2.4

        String readPath = "xbars/var5.xbar";
        String writePath = "xbar_results.txt";
        Xbar xbar = new Xbar(readPath); // Task 1.3
        xbar.printXbar(); // Task 1.4
        xbar.writeXbarToFile(writePath); // Task 1.5
        xbar.evaluateAllXbarInstances(); // Task 2.1 and Task 2.2
    }
}
