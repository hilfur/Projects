package main;

import java.io.*;
import java.util.Scanner;
class BinærtSøketre
{
	private static String x; 
	static String[] splitta; 
	static String teller[];
	
	public static class node {
		String ord;
		int teller = 1; 
		node høyre,venstre;
		
		public node(String ting){
			ord = ting;
			høyre = venstre = null ;
		}
	}
	node root;
	private static Scanner scanner; 
	BinærtSøketre() {
		root = null;
	}
	void Leggtil(String ord){
		root = LeggtilRekursiv(root, ord);
	}
	node LeggtilRekursiv(node root, String ord){
		if (root == null){
			root = new node(ord);
			return root;
		}

		if (ord.compareTo(root.ord) < 0){
			root.venstre = LeggtilRekursiv(root.venstre, ord);
		}
		else if(ord.compareTo(root.ord) > 0){
			root.høyre = LeggtilRekursiv(root.høyre, ord);
		}
		else if(ord.compareTo(root.ord) == 0){
			root.teller ++; 
		}
		return root;
	}
	void indorder(){
		inorderRekursive(root);
	}
	void inorderRekursive(node root){
		if(root != null){
			inorderRekursive(root.venstre);
			System.out.println(root.ord + " " + root.teller);
			inorderRekursive(root.høyre);
		}
	}
	
	
	
	
	
	
	
    public static void main (String argv[]) throws FileNotFoundException{
    	BinærtSøketre tre = new BinærtSøketre();
    	
    	
    	scanner = new Scanner(System.in); 
    	System.out.println("Programmet leser bare fra filer... ");
    	System.out.println("Skriv filnavn på fil husk ekstra \\ når du skriver");
    	x = scanner.nextLine();
    	Lesfrafil(x);
    	for(int i = 0; i<splitta.length; i++ ){
    		tre.Leggtil(splitta[i]);
    	}
    	tre.indorder();

    }
    public static void Lesfrafil(String filplassering) throws FileNotFoundException{
    	BufferedReader r = 
    		    new BufferedReader(new FileReader(x));
    		StreamTokenizer input = new StreamTokenizer(r);

    		String Strengen = "";
    		try
    		{
    		    int x = input.nextToken();
    		    while (x != input.TT_EOF)
    		    {
    			if (input.ttype == input.TT_WORD)
    				Strengen += input.sval + " ";
    			x = input.nextToken ();
    		    }
    		}
    		catch (IOException e) {};
    		String PreSplit = Strengen.replaceAll("\\.", "").toUpperCase();
    		splitta = PreSplit.split(" ");
    }
    
}