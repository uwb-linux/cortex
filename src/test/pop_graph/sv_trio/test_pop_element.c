#include <CUnit.h>
#include <Basic.h>
#include <element.h>
#include <open_hash/hash_table.h>
#include "test_pop_element.h"
#include <stdlib.h>
#include <stdio.h>

void test_get_edge()
{

  Element* my_element=new_element();

  my_element->individual_edges[0]=1;
  my_element->individual_edges[1]=3;

  Edges* edges=get_edge( *my_element, individual_edge_array, 0);
  CU_ASSERT(*edges==1);
  edges=get_edge( *my_element, individual_edge_array, 1);
  CU_ASSERT(*edges==3);

  free_element(&my_element);




  my_element=new_element();
 
  my_element->individual_edges[0]=2;
  my_element->individual_edges[1]=4;

  edges=get_edge( *my_element, individual_edge_array, 0);
  CU_ASSERT(*edges==2);
  edges=get_edge( *my_element, individual_edge_array, 1);
  CU_ASSERT(*edges==4);

  free_element(&my_element);


}



void test_get_edge_copy()
{

  Element* my_element=new_element();

  my_element->individual_edges[0]=1;
  my_element->individual_edges[1]=3;

  Edges edges=get_edge_copy( *my_element, individual_edge_array, 0);
  CU_ASSERT(edges==1);
  edges=get_edge_copy( *my_element, individual_edge_array, 1);
  CU_ASSERT(edges==3);

  free_element(&my_element);




  my_element=new_element();
 
  my_element->individual_edges[0]=2;
  my_element->individual_edges[1]=4;

  edges=get_edge_copy( *my_element, individual_edge_array, 0);
  CU_ASSERT(edges==2);
  edges=get_edge_copy( *my_element, individual_edge_array, 1);
  CU_ASSERT(edges==4);

  free_element(&my_element);


}


void test_increment_coverage()
{
  dBNode* e = new_element();
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==1);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==2);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==3);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==4);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==5);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==6);
  CU_ASSERT(e->coverage[1]==0);
  CU_ASSERT(e->coverage[2]==0);

  e->coverage[0]=(unsigned char) 251;  // note you have to be careful just assigning an int to a char
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==252);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==253);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==254);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==255);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(e->coverage[0]==256);
  db_node_increment_coverage(e,individual_edge_array,0);
 
  
  free_element(&e);
}

void test_get_coverage()
{
  dBNode* e = new_element();
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==1);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==2);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==3);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==4);
  db_node_increment_coverage(e,individual_edge_array,0);
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==5);
  
  e->coverage[0]=120;
  CU_ASSERT(db_node_get_coverage(e,individual_edge_array,0)==120);
  
  free_element(&e);
}


void test_element_status_set_and_checks()
{
  dBNode* e = new_element();
  
  db_node_set_status(e, visited);
  CU_ASSERT(db_node_check_status(e, visited)==true);
  CU_ASSERT(db_node_check_status(e, none)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==false);



  db_node_set_status(e, none);
  CU_ASSERT(db_node_check_status(e, none)==true);
  CU_ASSERT(db_node_check_status(e, visited)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==false);



  db_node_set_status(e, pruned_from_NA12878);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878)==true);
  CU_ASSERT(db_node_check_status(e, none)==false);
  CU_ASSERT(db_node_check_status(e, visited)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==false);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==false);




  db_node_set_status(e, pruned_from_NA12891);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891)==true);
  db_node_set_status(e, pruned_from_NA12892);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12892)==true);
  db_node_set_status(e, pruned_from_NA12878_and_NA12892);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12892)==true);
  db_node_set_status(e, pruned_from_NA12878_and_NA12891);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891)==true);
  db_node_set_status(e, pruned_from_NA12891_and_NA12892);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==true);
  db_node_set_status(e, pruned_from_NA12878_and_NA12891_and_NA12892);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==true);

  db_node_set_status_to_none(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  //important that at this point we have set status to none
  db_node_trio_aware_set_pruned_status(e, 0);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878)==true);
  db_node_set_status_to_none(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  db_node_trio_aware_set_pruned_status(e, 1);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891)==true);
  db_node_set_status_to_none(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  db_node_trio_aware_set_pruned_status(e, 2);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12892)==true);
  db_node_set_status_to_none(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  db_node_set_status(e, pruned_from_NA12878_and_NA12892);
  db_node_trio_aware_set_pruned_status(e, 1);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==true);

  db_node_set_status_to_none(e);
  db_node_set_status(e, pruned_from_NA12878_and_NA12891);
  db_node_trio_aware_set_pruned_status(e, 2);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==true);

  db_node_set_status_to_none(e);
  db_node_set_status(e, pruned_from_NA12891_and_NA12892);
  db_node_trio_aware_set_pruned_status(e, 0);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12878_and_NA12891_and_NA12892)==true);


  db_node_set_status_to_none(e);
  db_node_set_status(e, pruned_from_NA12891_and_NA12892);
  db_node_trio_aware_set_pruned_status(e, 1);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==true);
  db_node_trio_aware_set_pruned_status(e, 2);
  CU_ASSERT(db_node_check_status(e, pruned_from_NA12891_and_NA12892)==true);


  db_node_action_set_status_none(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  db_node_action_set_status_visited(e);
  CU_ASSERT(db_node_check_status(e, visited)==true);
  db_node_action_set_status_visited_or_visited_and_exists_in_reference(e);
  CU_ASSERT(db_node_check_status(e, visited)==true);
  db_node_action_unset_status_visited_or_visited_and_exists_in_reference(e);
  CU_ASSERT(db_node_check_status(e, none)==true);

  
  db_node_set_status(e,exists_in_reference);
  CU_ASSERT(db_node_check_status(e, exists_in_reference)==true);
  db_node_action_set_status_visited_or_visited_and_exists_in_reference(e);
  CU_ASSERT(db_node_check_status(e, visited_and_exists_in_reference)==true);
  db_node_action_unset_status_visited_or_visited_and_exists_in_reference(e);
  CU_ASSERT(db_node_check_status(e, exists_in_reference)==true);


  db_node_set_status(e,exists_in_reference);
  CU_ASSERT(!db_node_check_status_is_not_exists_in_reference(e));
  db_node_set_status(e,visited_and_exists_in_reference);
  CU_ASSERT(db_node_check_status_is_not_exists_in_reference(e)); //note we are being sematically strict here - the STATUS is not exists_in_reference, even though the kmer IS in the reference
  db_node_set_status(e,none);
  CU_ASSERT(db_node_check_status_is_not_exists_in_reference(e));
  db_node_set_status(e,visited);
  CU_ASSERT(db_node_check_status_is_not_exists_in_reference(e));
  db_node_set_status(e,pruned_from_NA12878);
  CU_ASSERT(db_node_check_status_is_not_exists_in_reference(e));

  db_node_set_status_to_none(e);
  CU_ASSERT(db_node_check_status_is_not_visited_or_visited_and_exists_in_reference(e));
  db_node_set_status(e,visited);
  CU_ASSERT(!db_node_check_status_is_not_visited_or_visited_and_exists_in_reference(e));
  db_node_set_status(e, visited_and_exists_in_reference);
  CU_ASSERT(!db_node_check_status_is_not_visited_or_visited_and_exists_in_reference(e));
  

  free_element(&e);


  
}
