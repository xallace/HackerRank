def main():
    T = int(input())
    for _ in range(T):
        A = input().strip()
        B = input().strip()
        result = []

        i = j = 0
        while i < len(A) or j < len(B):
            k, l = i, j
            while k < len(A) and l < len(B) and A[k] == B[l]:
                k += 1
                l += 1

            if k == len(A):
                l = j
                while j < len(B) and B[j] == B[l]:
                    result.append(B[j])
                    j += 1
            elif l == len(B):
                k = i
                while i < len(A) and A[i] == A[k]:
                    result.append(A[i])
                    i += 1
            elif A[k] < B[l]:
                k = i
                while i < len(A) and A[i] == A[k]:
                    result.append(A[i])
                    i += 1
            else:
                l = j
                while j < len(B) and B[j] == B[l]:
                    result.append(B[j])
                    j += 1

        print(''.join(result))


if __name__ == "__main__":
    main()
