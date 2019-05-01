package main;

import java.text.DecimalFormat;
import java.util.Random;
import java.util.Scanner;


import jsjf.CircularArrayQueue;

public class TestAvSort {
	private static int n; 
	private static int inputvalg; 
	static long time = 0; 
/* Mye av koden har blitt hentet fra kildekoden fra læreboka og fra forelesningene. Quicksort og Mergesort har blitt hentet fra en annen plass på nettet.
 * 	Regnet med at det ikke var noe problem å hente kode fra et annet sted, men brukte Quicksort og Mergesort fra andre kilder ettersom jeg allerede var der 
 * 	og fikk repetisjon på hvordan de forskjellige fungerte. 
 * 
 * Quicksort:  https://www.geeksforgeeks.org/quick-sort/
 * Mergesort: https://www.geeksforgeeks.org/merge-sort/ 
 * Radixsort: Hentet fra læreboka/forelesninger
 * 
 * 
 * Programmet har ikke så veldig mange input sjekker på om det er riktig eller feil.
 * 
 * */
	private static Scanner scanner;
	
	public static void main(String arg[]){
		while(inputvalg != 9){
		scanner = new Scanner(System.in);
		Random r = new Random();
		long time = 0; 
		System.out.println("Antall tall som skal legges til i Array?");
		n = scanner.nextInt(); 
		System.out.println("Type sorteringsmetode?");

		int[] a = new int[n];
		for(int x = 0; x<n; x++){
				a[x] = r.nextInt(n);
		}
		//Beregner maksSiffer til Radixsorteringen
		int maksSiffer = (int)(Math.log10(a.length)+1);
		System.out.println("1 = Insertion Sort, 2 = Quicksort, 3 = Merge Sort, 4 = Radixsort, 9 = exit");
		inputvalg = scanner.nextInt();
		System.out.println("1 = Skal programmet utføre sortering og vise kjøretid, 2 = estimere C, 3 = begge");
		int inputvalg2 = scanner.nextInt();
		if(inputvalg == 1){			
			//System.out.println(Arrays.toString(a));
			if(inputvalg2 == 1 || inputvalg2 == 3){
			time = System.currentTimeMillis();
			Innstikksortering(a);
			time = System.currentTimeMillis() - time;
			//System.out.println(Arrays.toString(a));
			System.out.println("Insertion sort: " + time + "ms" );
			}
			
			if(inputvalg2 == 2 || inputvalg2 == 3){
			long estimat = estimat(inputvalg, maksSiffer);
			System.out.println("Estimert C = " + (float) estimat/(n*n));
			}
		}
		else if(inputvalg == 2){
			if(inputvalg2 == 1 || inputvalg2 == 3){
			//System.out.println(Arrays.toString(a));
			time = System.currentTimeMillis();
			TestAvSort.quicksort(a, 0, a.length-1);
			time = System.currentTimeMillis() - time;
			//System.out.println(Arrays.toString(a));
			System.out.println("Quicksort: " + time/1000.0 + " ms");
			}
			if(inputvalg2 == 2 || inputvalg2 == 3){
			long estimat = estimat(inputvalg, maksSiffer);
			System.out.println("Estimert C=" + (float) estimat/(n *Math.log(n)));
			}
			
		}
		else if (inputvalg == 3){
			if(inputvalg2 == 1 || inputvalg2 == 3){
			//System.out.println(Arrays.toString(a));
			time = System.currentTimeMillis();
			TestAvSort.mergesort(a, 0, a.length-1);
			time = System.currentTimeMillis() - time;
			//System.out.println(Arrays.toString(a));
			System.out.println("Mergesort: " + time/1000.0 + " ms");
			}
			if(inputvalg2 == 2 || inputvalg2 == 3){
			long estimat = estimat(inputvalg, maksSiffer);
			System.out.println("Estimert C=" + (float) estimat/(n *Math.log(n)));
			}
		}
		else if(inputvalg == 4){
			if(inputvalg2 == 1 || inputvalg2 == 3){
			//System.out.println(Arrays.toString(a));
			time = System.currentTimeMillis();
			RadixSortering(a, maksSiffer);
			time = System.currentTimeMillis() - time;
			//System.out.println(Arrays.toString(a));
			System.out.println("Radixsort: " + time/1000.0 + " ms");
			}
			if(inputvalg2 == 2 || inputvalg2 == 3){
			long estimat = estimat(inputvalg, maksSiffer);
			System.out.println("Estimert C=" + (float) estimat/n);
			}
		}
		else if (inputvalg != 9){
			System.out.println("Feil input prøv igjen");
		}
		}
		System.out.println("Avslutter...");
		System.exit(0);
	}
	
	
	 public static void Innstikksortering(int a[])
 {	        int n = a.length;
	        for (int index=1; index<n; index++)
	        {
	        	int j = index-1;
	            int f = a[index];
	            while (j>=0 && a[j] > f)
	            {
	                a[j+1] = a[j];
	                j = j-1;
	            }
	            a[j+1] = f;
	        }
	    }
	 
	 public static void RadixSortering(int a[], int maksSiffer){

			int ti_i_m = 1;
			int n = a.length;

			// Oppretter 10 tomme kÃ¸er 
			CircularArrayQueue<Integer>[] Q = 
			    (CircularArrayQueue<Integer>[])(new CircularArrayQueue[10]);

			for (int i = 0; i < 10; i++)
			    Q[i] = new CircularArrayQueue<Integer>(); 
		            

			for (int m = 0; m < maksSiffer; m++)
			{
			    for (int i = 0; i < n; i++)
			    {
				int siffer = (a[i] / ti_i_m) % 10;
				Q[siffer].enqueue(new Integer(a[i]));
			    }

			    int j = 0;
			    for (int i = 0; i < 10; i++)
				while (!Q[i].isEmpty())
				    a[j++] = (int) Q[i].dequeue();

			    ti_i_m *= 10;
			}
	 }
	 
	 
	private static int partition(int[] a, int low, int high)

	    {
        int pivot = a[high]; 
        int i = (low-1);
        for (int j=low; j<high; j++)
        {

            if (a[j] <= pivot)
            {
                i++;
 
                swap(a, i, j);
            }
        }
		
		swap(a, i+1, high);
		
		return i+1;
	    } 
	private static void quicksort(int a[], int low, int high)
	    {
		if (low < high)
		{

			int indexofpartition = partition(a, low, high);
			
			
			quicksort(a, low, indexofpartition - 1);
			
			
			quicksort(a, indexofpartition + 1, high);
	        }
	    }
	 
	 static void merge(int a[], int min, int mid, int max)
	    {
	        // Find sizes of two subarrays to be merged
	        int n1 = mid - min + 1;
	        int n2 = max - mid;
	 
	        /* Create temp arrays */
	        int L[] = new int [n1];
	        int R[] = new int [n2];
	 
	        /*Copy data to temp arrays*/
	        for (int i=0; i<n1; ++i)
	            L[i] = a[min + i];
	        for (int j=0; j<n2; ++j)
	            R[j] = a[mid + 1+ j];
	 
	 
	        /* Merge the temp arrays */
	 
	        // Initial indexes of first and second subarrays
	        int i = 0, j = 0;
	 
	        // Initial index of merged subarry array
	        int tempMin = min;
	        while (i < n1 && j < n2)
	        {
	            if (L[i] <= R[j])
	            {
	                a[tempMin] = L[i];
	                i++;
	            }
	            else
	            {
	                a[tempMin] = R[j];
	                j++;
	            }
	            tempMin++;
	        }
	 
	        /* Copy remaining elements of L[] if any */
	        while (i < n1)
	        {
	            a[tempMin] = L[i];
	            i++;
	            tempMin++;
	        }
	 
	        /* Copy remaining elements of R[] if any */
	        while (j < n2)
	        {
	            a[tempMin] = R[j];
	            j++;
	            tempMin++;
	        }
	    }
	 static void mergesort(int a[], int min, int max)
	    {
	        if (min < max)
	        {
	            // Find the middle point
	            int mid = (min+max)/2;
	 
	            // Sort first and second halves
	            mergesort(a, min, mid);
	            mergesort(a , mid+1, max);
	 
	            // Merge the sorted halves
	            merge(a, min, mid, max);
	        }
	    }
	 
	 //Swap
	 private static void swap(int a[], int index1, int index2){
		 int temp = a[index1];
		 a[index1] = a[index2];
		 a[index2] = temp; 
	 }
	 
	 private static long estimat(int z, int maksSiffer){
		 Random r = new Random();
		 long c = 0; 
		 long gjennomsnitt = 0;
		 for (int n = 1000; n <= 10000; n += 1000)
		 {
		    int b[] = new int[n];
		    long time = 0;
	    
		    for (int i = 0; i < n; i++)
			b[i] = r.nextInt(n);
		    
		    if(z == 1){
		    time = System.currentTimeMillis();
		    Innstikksortering(b);
		    time = System.currentTimeMillis() - time;
		    }
		    else if(z == 2){
		    	time = System.currentTimeMillis();
		    	TestAvSort.quicksort(b, 0, b.length-1);
		    	time = System.currentTimeMillis() - time;
		    }
		    else if(z == 3){
				time = System.currentTimeMillis();
				TestAvSort.mergesort(b, 0, b.length-1);
				time = System.currentTimeMillis() - time;
		    }
		    else if (z == 4){
				time = System.currentTimeMillis();
				RadixSortering(b, maksSiffer);
				time = System.currentTimeMillis() - time;
		    }
		    
		    gjennomsnitt += time;
		 }
	     for (int g = 20000; g <= 50000; g += 10000)
		 {
		    int b[] = new int[g];
		    long time = 0;
	    
		    for (int i = 0; i < g; i++)
			b[i] = r.nextInt(g);

		    
		    
		    if(z == 1){
		    time = System.currentTimeMillis();
		    Innstikksortering(b);
		    time = System.currentTimeMillis() - time;
		    }
		    else if(z == 2){
		    	time = System.currentTimeMillis();
		    	TestAvSort.quicksort(b, 0, b.length-1);
		    	time = System.currentTimeMillis() - time;
		    }
		    else if(z == 3){
				time = System.currentTimeMillis();
				TestAvSort.mergesort(b, 0, b.length-1);
				time = System.currentTimeMillis() - time;
		    }
		    else if (z == 4){
				time = System.currentTimeMillis();
				RadixSortering(b, maksSiffer);
				time = System.currentTimeMillis() - time;
		    }
		    
		    gjennomsnitt += time;
	     }
	     c= gjennomsnitt/15;
	     return c; 
	 }
}
