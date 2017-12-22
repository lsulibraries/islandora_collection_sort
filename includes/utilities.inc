<?php

/**
 * Implements callback_islandora_basic_collection_query_backends().
 */
function islandora_collection_sort_islandora_basic_collection_backend_callable($collection_object, $page, $limit) {
  $key = get_collection_cache_key($collection_object->id, $page, $limit);
  $cache = cache_get($key);
  if ($cache) {
    $to_return = $cache->data;
  }
  else {
    $to_return = list($total_count, $pids) = islandora_collection_sort_get_sorted_items($collection_object, $page, $limit);
    cache_set($key, $to_return);
  }
  return $to_return;
}

function islandora_collection_sort_get_sorted_items($collection_object, $page, $limit) {
  list($total_count, $results) = islandora_basic_collection_get_member_objects($collection_object, -1, -1);
  uasort($results, function($a, $b) {
    return strnatcasecmp($a['title']['value'], $b['title']['value']);
  });

  $offset = $page * $limit;
  $pids = array();
  foreach (array_slice($results, $offset, $limit) as $result) {
    $pids[] = islandora_object_load($result['object']['value']);
  }
  return array($total_count, $pids);
}

function clear_collection_counts_cache($collection_pid) {
  cache_clear_all(get_collection_cache_key($collection_pid), 'cache', TRUE);
}

function get_collection_cache_key($collection_pid, $page = NULL, $limit = NULL) {
  $components = array(
    'islandora_collection_sort',
    $collection_pid,
    (string) $page,
    (string) $limit,
  );
  return implode('_', array_filter($components));
}
