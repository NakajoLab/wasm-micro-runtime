#ifndef NATIVEFUNC_H
#define NATIVEFUNC_H

int nmpz_mul( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_tdiv_q( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_add(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
  int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
  int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_sub(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_mod(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_gcd(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_invert(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int nmpz_nextprime( int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1);

int nmpz_powm(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2,
    int _mp_alloc_3, int _mp_size_3, unsigned long int *_mp_d_3);

int nmpz_sub_ui(int *_mp_alloc_rop,  int *_mp_size_rop, unsigned long int *_mp_d_rop,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    unsigned long int _mp_d_2);

int nmpz_gcdext( int *_mp_alloc_rop_1,  int *_mp_size_rop_1, unsigned long int *_mp_d_rop_1,
    int *_mp_alloc_rop_2,  int *_mp_size_rop_2, unsigned long int *_mp_d_rop_2,
    int *_mp_alloc_rop_3,  int *_mp_size_rop_3, unsigned long int *_mp_d_rop_3,
    int _mp_alloc_1, int _mp_size_1, unsigned long int *_mp_d_1,
    int _mp_alloc_2, int _mp_size_2, unsigned long int *_mp_d_2);

int wrapped_nmpz_mul(mpz_t rop, mpz_t op1, mpz_t op2)
{
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + abs(op2->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_mul(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
}

int wrapped_nmpz_tdiv_q(mpz_t rop, mpz_t op1, mpz_t op2)
{
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) +  2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_tdiv_q(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
}

int wrapped_nmpz_add(mpz_t rop, mpz_t op1, mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
      mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }
  
    int alloc_size = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? (abs(op1->_mp_size)+1) : abs(op2->_mp_size)) + 2;
    
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);
    
    nmpz_add(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
        
    mpz_realloc(rop, rop->_mp_size);
  
    return 0;
  }

  int wrapped_nmpz_sub(mpz_t rop, mpz_t op1, mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
      mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }
  
    int alloc_size = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);
    
    nmpz_sub(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
        
    mpz_realloc(rop, rop->_mp_size);
  
    return 0;
  }

  int wrapped_nmpz_mod(mpz_t rop, mpz_t op1, mpz_t op2)
  {
      if(op1->_mp_alloc % 2 != 0){
          mpz_realloc(op1, op1->_mp_alloc+1);
      }
      if(op2->_mp_alloc % 2 != 0){
          mpz_realloc(op2, op2->_mp_alloc+1);
      }
  
      int alloc_size = abs(op2->_mp_size) + 2;
      if(alloc_size  % 2 != 0){
          alloc_size++;
      }
      mpz_realloc(rop, alloc_size);
  
      nmpz_mod(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
          op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
          op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size); 
    return 0;
  }

int wrapped_nmpz_gcd(mpz_t rop,mpz_t op1,mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_gcd(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
  }

int wrapped_nmpz_invert(mpz_t rop, mpz_t op1, mpz_t op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    int alloc_size = abs(op2->_mp_size) + 1 + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    int res = nmpz_invert(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return res;
  }

int wrapped_nmpz_nextprime(mpz_t rop, mpz_t op1){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + abs(op1->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_nextprime(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
}

int wrapped_nmpz_powm(mpz_t rop,  mpz_t op1,  mpz_t op2,  mpz_t op3){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }
    if(op3->_mp_alloc % 2 != 0){
        mpz_realloc(op3, op3->_mp_alloc+1);
    }

    int alloc_size = abs(op3->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_powm(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d,
        op3->_mp_alloc, op3->_mp_size, op3->_mp_d);
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
}

int wrapped_nmpz_sub_ui(mpz_t rop,  mpz_t op1,  unsigned long int op2){
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }

    int alloc_size = abs(op1->_mp_size) + 2;
    if(alloc_size  % 2 != 0){
        alloc_size++;
    }
    mpz_realloc(rop, alloc_size);

    nmpz_sub_ui(&rop->_mp_alloc, &rop->_mp_size, rop->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2
    );
    
    mpz_realloc(rop, rop->_mp_size);
    return 0;
}

 

int wrapped_nmpz_gcdext(mpz_t rop1, mpz_t rop2, mpz_t rop3, mpz_t op1, mpz_t op2)
{
    if(op1->_mp_alloc % 2 != 0){
        mpz_realloc(op1, op1->_mp_alloc+1);
    }
    if(op2->_mp_alloc % 2 != 0){
        mpz_realloc(op2, op2->_mp_alloc+1);
    }

    // int alloc_size1 = min(abs(op1->_mp_size) , abs(op2->_mp_size)) + 2;
    int alloc_size1 = ((abs(op1->_mp_size) < abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    if(alloc_size1  % 2 != 0){
        alloc_size1++;
    }
    mpz_realloc(rop1, alloc_size1);

    int alloc_size2 = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    if(alloc_size2  % 2 != 0){
        alloc_size2++;
    }
    mpz_realloc(rop2, alloc_size2);

    int alloc_size3 = ((abs(op1->_mp_size) > abs(op2->_mp_size)) ? abs(op1->_mp_size) : abs(op2->_mp_size)) + 2;
    if(alloc_size3  % 2 != 0){
        alloc_size3++;
    }
    mpz_realloc(rop3, alloc_size3);

    nmpz_gcdext(&rop1->_mp_alloc, &rop1->_mp_size, rop1->_mp_d,
        &rop2->_mp_alloc, &rop2->_mp_size, rop2->_mp_d,
        &rop3->_mp_alloc, &rop3->_mp_size, rop3->_mp_d,
        op1->_mp_alloc, op1->_mp_size, op1->_mp_d,
        op2->_mp_alloc, op2->_mp_size, op2->_mp_d);
    
    mpz_realloc(rop1, rop1->_mp_size);
    mpz_realloc(rop2, rop2->_mp_size);
    mpz_realloc(rop3, rop3->_mp_size);
    return 0;
}

#endif