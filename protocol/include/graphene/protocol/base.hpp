#
#pragma once

#include <graphene/protocol/ext.hpp>
#include <graphene/protocol/types.hpp>

#include <fc/thread/future.hpp>

namespace graphene { namespace protocol {
   struct asset;
   struct authority;

   /**
    *  @defgroup operations Operations
    *  @ingroup transactions Transactions
    *  @brief A set of valid comands for mutating the globally shared state.
    *
    *  An operation can be thought of like a function that will modify the global
    *  shared state of the blockchain.  The members of each struct are like function
    *  arguments and each operation can potentially generate a return value.
    *
    *  Operations can be grouped into transactions (@ref transaction) to ensure that they occur
    *  in a particular order and that all operations apply successfully or
    *  no operations apply.
    *
    *  Each operation is a fully defined state transition and can exist in a transaction on its own.
    *
    *  @section operation_design_principles Design Principles
    *
    *  Operations have been carefully designed to include all of the information necessary to
    *  interpret them outside the context of the blockchain.   This means that information about
    *  current chain state is included in the operation even though it could be inferred from
    *  a subset of the data.   This makes the expected outcome of each operation well defined and
    *  easily understood without access to chain state.
    *
    *  @subsection balance_calculation Balance Calculation Principle
    *
    *    We have stipulated that the current account balance may be entirely calculated from
    *    just the subset of operations that are relevant to that account.  There should be
    *    no need to process the entire blockchain inorder to know your account's balance.
    *
    *  @subsection fee_calculation Explicit Fee Principle
    *
    *    Blockchain fees can change from time to time and it is important that a signed
    *    transaction explicitly agree to the fees it will be paying.  This aids with account
    *    balance updates and ensures that the sender agreed to the fee prior to making the
    *    transaction.
    *
    *  @subsection defined_authority Explicit Authority
    *
    *    Each operation shall contain enough information to know which accounts must authorize
    *    the operation.  This principle enables authority verification to occur in a centralized,
    *    optimized, and parallel manner.
    *
    *  @subsection relevancy_principle Explicit Relevant Accounts
    *
    *    Each operation contains enough information to enumerate all accounts for which the
    *    operation should apear in its account history.  This principle enables us to easily
    *    define and enforce the @balance_calculation. This is superset of the @ref defined_authority
    *
    *  @{
    */

   struct void_result{};
   typedef fc::static_variant<void_result,object_id_type,asset> operation_result;

   struct base_operation
   {
      template<typename T>
      share_type calculate_fee(const T& params)const
      {
         return params.fee;
      }
      void get_required_authorities( vector<authority>& )const{}
      void get_required_active_authorities( flat_set<account_id_type>& )const{}
      void get_required_owner_authorities( flat_set<account_id_type>& )const{}
      void validate()const{}
      fc::optional< fc::future<void> > validate_parallel( uint32_t skip )const;

      static uint64_t calculate_data_fee( uint64_t bytes, uint64_t price_per_kbyte );
   };

   /**
    *  For future expansion many structus include a single member of type
    *  extensions_type that can be changed when updating a protocol.  You can
    *  always add new types to a static_variant without breaking backward
    *  compatibility.   
    */
   typedef static_variant<void_t>      future_extensions;

   /**
    *  A flat_set is used to make sure that only one extension of
    *  each type is added and that they are added in order.  
    *  
    *  @note static_variant compares only the type tag and not the 
    *  content.
    */
   typedef flat_set<future_extensions> extensions_type;

   ///@}

} } // graphene::protocol

FC_REFLECT_TYPENAME( graphene::protocol::operation_result )
FC_REFLECT_TYPENAME( graphene::protocol::future_extensions )
FC_REFLECT( graphene::protocol::void_result, )
