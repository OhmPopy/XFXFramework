/********************************************************
 *	Dictionary.h										*
 *														*
 *	XFX Generic Dictionary class definition file		*
 *	Copyright � XFX Team. All Rights Reserved			*
 ********************************************************/
#ifndef _SYSTEM_COLLECTIONS_GENERIC_DICTIONARY_
#define _SYSTEM_COLLECTIONS_GENERIC_DICTIONARY_

#include <System/Array.h>
#include <System/Exception.h>
#include <System/Collections/Generic/Dictionary.h>
#include <System/Collections/Generic/EqualityComparer.h>
#include "HashHelpers.h"
#include "Interfaces.h"
#include "KeyValuePair.h"

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			/// <summary>
			/// Represents a collection of keys and values.
			/// </summary>
			template <class TKey, class TValue>
			class Dictionary : public IDictionary<TKey, TValue>, public ICollection<KeyValuePair<TKey, TValue> >, virtual Object
			{
			private:
				template <class UKey, class UValue>
				struct Entry
				{
					int hashCode;
					int next;
					UKey key;
					UValue value;
				};

			private:
				int buckets[];
				static const char* ComparerName;
				IEqualityComparer<TKey>* comparer;
				int count;
				Entry<TKey, TValue> entries[];
				int freeCount;
				int freeList;
				static const char* HashSizeName;
				static const char* KeyValuePairsName;
				int version;
				static const char* VersionName;

				void Add(KeyValuePair<TKey, TValue> keyValuePair);
				bool Contains(KeyValuePair<TKey, TValue> keyValuePair);
				void CopyTo(KeyValuePair<TKey, TValue> array[], int index);
				int FindEntry(TKey key);
				bool Remove(KeyValuePair<TKey, TValue> keyValuePair);
				void Initialize(int capacity);
				void Insert(TKey key, TValue value, bool add);
				void Resize();

			public:
				/// <summary>
				/// Represents the collection of keys in a Dictionary<,>. 
				/// </summary>
				template <class UKey, class UValue>
				class KeyCollection : public ICollection<UKey>, public IEnumerable<UKey>
				{
				private:
					Dictionary<TKey, TValue>* _dictionary;

				public:
					int Count()
					{
						return _dictionary.Count();
					}

					KeyCollection(Dictionary<TKey, TValue> dictionary)
					{
						_dictionary = &dictionary;
					}

					KeyCollection(const KeyCollection<UKey, UValue> &obj)
					{
						_dictionary = &obj._dictionary;
					}

					void Add(UKey item)
					{
						throw NotSupportedException("Adding keys directly to the Dictionary::Keycollection is not supported.");
					}

					void Clear()
					{
						throw NotSupportedException("Directly clearing the Dictionary::KeyCollection is not supported.");
					}

					bool Contains(UKey item)
					{
						return _dictionary.ContainsKey(item);
					}

					void CopyTo(UKey array[], int index)
					{
						if(array == null)
						{
							throw ArgumentNullException("array");
						}
						if((index < 0) ||(index > Array::Length(array)))
						{
							throw ArgumentOutOfRangeException("index", "Non-negative array index required.");
						}
						if((Array::Length(array) - index) < _dictionary.Count())
						{
							throw ArgumentException("Array plus offset too small.");
						}
						int count = _dictionary.Count();
						Entry<UKey, UValue> entries[] = _dictionary.entries;
						for(int i = 0; i < count; i++)
						{
							if(entries[i].hashCode >= 0)
							{
								array[index++] = entries[i].key;
							}
						}
					}

					bool Remove(UKey item)
					{
						throw NotSupportedException("Removing keys directly from the Dictionary::KeyCollection is not supported.");
						return false;
					}
				};

				/// <summary>
				/// Represents the collection of values in a Dictionary<,>.
				/// </summary>
				template <class UKey, class UValue>
				class ValueCollection : public ICollection<UValue>, public IEnumerable<UValue>
				{
				private:
					Dictionary<TKey, TValue> *_dictionary;

				public:
					int Count();

					ValueCollection(Dictionary<TKey, TValue> dictionary);
					ValueCollection(const ValueCollection<UKey, UValue> &obj);

					void Add(UValue item);
					void Clear();
					bool Contains(UValue item);
					void CopyTo(UValue array[], int index);
					bool Remove(UValue item);
				};

			public:
				IEqualityComparer<TKey>* Comparer()
				{
					return comparer;
				}

				int Count();
				KeyCollection<TKey, TValue> Keys();
				ValueCollection<TKey, TValue> Values();
				TValue operator[](TKey key);

				Dictionary();
				Dictionary(IDictionary<TKey, TValue>* dictionary)
				{
					/*foreach (KeyValuePair<TKey, TValue> pair in dictionary)
					{
						Add(pair.Key, pair.Value);
					}*/
				}
				Dictionary(int capacity);
				~Dictionary()
				{
					delete[] buckets;
					delete[] entries;
				}

				void Add(TKey key, TValue value)
				{
					Insert(key, value, true);
				}

				void Clear()
				{
					if(count > 0)
					{
						for(int i = 0; i < Array::Length(buckets); i++)
						{
							buckets[i] = -1;
						}
						Array::Clear(entries, 0, count);
						freeList = -1;
						count = 0;
						freeCount = 0;
						version++;
					}
				}

				bool ContainsKey(TKey key);
				bool ContainsValue(TValue value);

				bool Remove(TKey key)
				{
					if(buckets)
					{
						int num = comparer->GetHashCode(key) & 0x7fffffff;
						int index = num % Array::Length(buckets);
						int num3 = -1;
						for(int i = buckets[index]; i >= 0; i = entries[i].next)
						{
							if((entries[i].hashCode == num) && comparer->Equals(entries[i].key, key))
							{
								if(num3 < 0)
								{
									buckets[index] = entries[i].next;
								}
								else
								{
									entries[num3].next = entries[i].next;
								}
								entries[i].hashCode = -1;
								entries[i].next = freeList;
								entries[i].key = TKey();
								entries[i].value = TValue();
								freeList = i;
								freeCount++;
								version++;
								return true;
							}
							num3 = i;
						}
					}
					return false;
				}

				bool TryGetValue(TKey key, out TValue value)
				{
					int index = FindEntry(key);
					if(index >= 0)
					{
						value = entries[index].value;
						return true;
					}
					value = TValue();
					return false;
				}
			};	
		}
	}
}

#endif //_SYSTEM_COLLECTIONS_GENERIC_DICTIONARY_
