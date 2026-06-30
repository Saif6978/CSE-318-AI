package util;

public class InversionCounter {
    public static int countInversionsSlow(int[] arr) {
        int n = arr.length;
        int c = 0;
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                if(arr[i] > arr[j]) c++;
            }
        }
        return c;
    }
    // this function counts inversions in O(nlogn)
    // it uses the merge sort for doing so
    // it counts the inversions while also performing merge sort
    // requires an extra 1D temp array
    public static int countInversions(int[] arr) {
        int[] temp = new int[arr.length];
        return mergeSortAndCountInversions(arr, temp, 0, arr.length - 1);
    }

    // recursive function
    private static int mergeSortAndCountInversions(int[] arr, int[] temp, int left, int right) {
        int mid, invCount = 0;
        if (left < right) {
            mid = (left + right) / 2;

            // left and right cases
            invCount += mergeSortAndCountInversions(arr, temp, left, mid);
            invCount += mergeSortAndCountInversions(arr, temp, mid + 1, right);

            invCount += mergeAndCount(arr, temp, left, mid, right);
        }
        return invCount;
    }

    // Merge two subarrays and count inversions
    private static int mergeAndCount(int[] arr, int[] temp, int left, int mid, int right) {
        // two pointers i and j
        int i = left;
        int j = mid + 1;

        // tracking the sorting in temp
        int k = left;

        int invCount = 0;

        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];

                // all the numbers in indices i...mid (including ends)
                // are greater than the number at index j
                // since the arrays are partially sorted in both parts
                invCount += (mid + 1) - i;
            }
        }

        // we have always taken the shorter numbers
        // so now all that remains are larger than all we have processed so far
        // now to whichever part they belong to, none to their left were
        // bigger than them, since the array are partially sorted
        // thus, now more inv count will be added
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        while (j <= right) {
            temp[k++] = arr[j++];
        }

        for (i = left; i <= right; i++) {
            arr[i] = temp[i];
        }

        return invCount;
    }

    public static void main(String[] args) {
        int[] arr = {1, 20, 6, 4, 5};
        System.out.println("Number of inversions: " + countInversions(arr));
    }
}

