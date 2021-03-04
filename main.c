#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>

int select_player_number();
int hand_out(int who);
void hitcard (void);
int hit_counter = 2;
void cards(int a);
void start(bool run, int cont);
int add_control_cards(double random[], int i ,int sum_of_cards);
int sum_player[2] = {[0 ... 1] = 0};
int sum[15] = {[0 ... 14] =0};
char name[50];
int money = 0;
char sit;
int bet = 0;
int player_number = 1;
FILE *fptr;

int main()
{
    fptr = fopen("blackJ_history.txt", "w");
    printf("\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n    %c%c%c%c%c Welcome to BLACKJACK %c%c%c%c%c\n\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,177,177,177,177,204,185,177,177,177,177,200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    int cont = 1;
    bool run = true;
    printf("Enter Your Name >> ");
    gets(name);
    fprintf(fptr, "User's Name is %s\n", name);
    start(run, cont);

    printf("\n");
    printf("            Thank You For Playing            \n");
    printf("\n");
    printf("       ------------See You------------\n");
    sleep(2);
    
    return 0;
}

void start(bool run, int cont)
{
    while (run)
    {

        int nop;
        nop = select_player_number();
        printf("Please place your bet $: ");
        scanf(" %d", &bet);
        fprintf(fptr, "%s bet $ %d\n", name , bet);

        sum_player[0] = hand_out(1);
        fprintf(fptr, "Sum of %s's cards = %d\n", name, sum_player[0]);

        printf("Hit or Stay?: ");
        scanf(" %c", &sit);
        while (sit == 'H' || sit == 'h')
        {
            hitcard();
            fprintf(fptr, "%s hit card, now sum is = %d\n", name, sum_player[0]);
            printf(" | Final = %d\n", sum_player[0]);
            if (sum_player[0] < 21)
            {
                printf("Hit or Stay?:");
                scanf(" %c", &sit);
            }
            if (sum_player[0] >= 21)
                sit = 's';
            hit_counter++;
        }
        fprintf(fptr,"%s stayed\n", name);

        sleep(1);
        for(int i=2; i < nop; i++)
        {
            sum[i] = hand_out(3);
            fprintf(fptr,"Sum of player %d is = %d\n", i, sum[i]);
            player_number++ ;
        }
        sum[1] = hand_out(2);
        fprintf(fptr, "Sum of dealers hand = %d", sum[1]);
        sleep(1);

        if (sum_player[0] > 21)
        {
            printf("You lost!!\n");
            money -= bet;
            fprintf(fptr,"%s lost $%d, hand is greater than 21\n", name, bet);
        }

        else
        {
            for(int i=1; i < nop; i++)
            {
                if (sum_player[0] < sum[i])
                {
                    printf("You Lost!!!\n");
                    money -= bet;
                    fprintf(fptr,"%s lost %d\n", name, bet);
                    goto jump;
                }
            }

            for(int i=1; i < nop; i++)
            {
                if (sum_player[0] == sum[i])
                {
                    printf("Draw!!!\n");
                    fprintf(fptr,"Draw Game\n");
                    goto jump;
                }
            }
                    printf("You Win!!!\n");
                    money += bet*nop;
            fprintf(fptr, "%s win %d\n", name, bet*nop);
        }

        sleep(1);
        jump:
        if (money < 0)
        {
            printf("Your loss >> $%d ...\n", -money);
            fprintf(fptr, "%s's loss is $%d \n" , name, -money);
        }
        else if (money > 0)
        {
            printf("Your profit >> $%d !!!\n", money);
            fprintf(fptr,"%s's profit is %d\n", name, money);
        }


        printf("Press 1 to play again:");
        scanf(" %d", &cont);
        if (cont == 1)
        {
            fprintf(fptr,"%s playing again\n", name);
            for(int i=1; i < nop; i++)
                sum[i] = 0;
            sum_player[0] = 0;
            player_number = 1;
        }
        else
        {
            fprintf(fptr, "%s quit the game\n", name);
            run = false;
            cont = 0;
        }

    }
}

int select_player_number()
{
    int pnmb;
    printf("Select a Table :\n\t<2>\n<7>\t\t<3>\n\n<6>\t\t<4>\n\t<5>\n>>");
    scanf(" %d", &pnmb);
    if(pnmb > 7 || pnmb <= 1)
    {
        printf("\aPlease Select Appropriate Table, ");
        return select_player_number();
    }
    return pnmb;
}

void hitcard ()
{
    srand(time(0));
    double hit[100];
    hit[hit_counter] = ((rand()  %11) +1) ;
    cards(hit[hit_counter]);
    sum_player[0] = add_control_cards(hit, hit_counter, sum_player[0]);
}


int hand_out(int who)
{
    int who_sum = 0;
    switch (who)
    {
        case 1:
            printf("%s's Hand:", name);
            break;
        case 2:
            printf("Dealer's Hand:");
            break;
        case 3:
            printf("Player %d:", player_number);
            break;
        default:
            break;
    }

    double ran[100];
    int i;

    for(i=0; i<2; i++)
    {
        srand(time(0));
        ran[i] = (rand() % 11) +1 ;
        cards(ran[i]);
        sleep(1);
        who_sum = add_control_cards(ran, i, who_sum);
    }

    if(who == 2 || who == 3)
    {
        while(who_sum < 17)
        {
            i++;
            srand(time(0));
            ran[i] = ((rand()  %11) +1);
            cards(ran[i]);
            sleep(1);
            who_sum = add_control_cards(ran, i, who_sum);
        }
        printf(" = %d", who_sum);
        printf("\n");
        if(who_sum>21)
        {
            who_sum = 0;
        }
        return who_sum;
    }
    else
    {
            printf("\n");
            return who_sum;
    }
}

void cards(int a)
{
    if (a>1 && a<10)
        printf(" %d", a);
    else if (a==10)
    {
        srand(time(0));
        int ran = ((rand()  %4) +1);
        char card_ten[3]= {'K','J','Q'};
        switch (ran)
        {
            case 1:
                printf(" %c", card_ten[0]);
                break;
            case 2:
                printf(" %c", card_ten[1]);
                break;
            case 3:
                printf(" %c", card_ten[2]);
                break;
            case 4:
                printf(" 10");
                break;
            default:
                break;
        }
    }
    else if (a==11 || a==1)
        printf(" A");
}

int add_control_cards(double random[], int i ,int sum_of_cards)
{

    if(random[i] == 11 || random[i] == 1)
    {
        if(sum_of_cards + random[i] > 21)
        {
            random[i] = 1;
            sum_of_cards += random[i];
        }
        else if(sum_of_cards + random[i] < 21)
        {
            random[i] = 11;
            sum_of_cards += random[i];
        }
        else if(sum_of_cards + random[i] ==21)
            sum_of_cards = 21;
    }
    else
        sum_of_cards += random[i];

    for(int j = 0; j <= i ; j++)
    {
        if(random[i] == 11 && sum_of_cards> 21)
            sum_of_cards -= 10;
    }

    return sum_of_cards;
}
