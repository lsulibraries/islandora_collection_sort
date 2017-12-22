<?php

/**
 * Implements callback_islandora_basic_collection_query_backends().
 */
function islandora_collection_sort_islandora_basic_collection_backend_callable($collection_object, $page, $limit) {

  $cache = cache_get($collection_object->id);
  if ($cache && isset($cache->data[$page]) && isset($cache->data[$page][$limit])) {
    $to_return = $cache->data[$page][$limit];
  }
  else {
    list($total_count, $results) = islandora_basic_collection_get_member_objects($collection_object, -1, -1);
    uasort($results, function($a, $b) {
      return strnatcasecmp($a['title']['value'], $b['title']['value']);
    });

    $offset = $page * $limit;
    $pids = array();
    foreach (array_slice($results, $offset, $limit) as $result) {
      $pids[] = islandora_object_load($result['object']['value']);
    }
    $to_return = array($total_count, $pids);
    $to_cache = array($page => array($limit => $to_return));
    cache_set($collection_object->id, $to_cache);
  }
  return $to_return;
}
